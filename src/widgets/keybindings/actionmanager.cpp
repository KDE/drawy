// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "actionmanager.hpp"

#include <KActionCollection>
#include <KMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <memory>
#include <utility>

#include "command/alignitemcommand.hpp"
#include "command/commandhistory.hpp"
#include "command/deselectcommand.hpp"
#include "command/groupcommand.hpp"
#include "command/removeitemcommand.hpp"
#include "command/selectcommand.hpp"
#include "command/ungroupcommand.hpp"
#include "command/zordercommand.hpp"
#include "common/constants.hpp"
#include "dialog/configuresettingsdialog.hpp"
#include "keybindmanager.hpp"

#include "components/propertybar.hpp"
#include "components/toolbar.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "drawy_debug.h"
#include "jobs/loadjobutil.hpp"
#include "jobs/saveasjob.hpp"
#include "serializer/serializerutils.hpp"
#include "serializer/svgserializer.hpp"

using namespace Qt::StringLiterals;
ActionManager::ActionManager(KActionCollection *actionCollection, ApplicationContext *context)
    : QObject(context)
    , m_context{context}
    , m_actionCollection(actionCollection)
{
    auto mainWindow{m_context->parentWidget()};

    KStandardActions::preferences(this, &ActionManager::configureSettings, actionCollection);
    KStandardActions::openNew(this, &ActionManager::newFile, actionCollection);
    KStandardActions::save(this, &ActionManager::saveCurrentFile, actionCollection);
    KStandardActions::saveAs(this, &ActionManager::saveAsNewFile, actionCollection);
    KStandardActions::open(this, &ActionManager::openFile, actionCollection);
    KStandardActions::undo(this, &ActionManager::undo, actionCollection)->setEnabled(false); // disabled initially
    KStandardActions::redo(this, &ActionManager::redo, actionCollection)->setEnabled(false); // disabled initially
    KStandardActions::zoomIn(this, &ActionManager::zoomIn, actionCollection)->setIcon(QIcon::fromTheme(u"value-increase"_s));
    KStandardActions::zoomOut(this, &ActionManager::zoomOut, actionCollection)->setIcon(QIcon::fromTheme(u"value-decrease"_s));
    KStandardActions::selectAll(this, &ActionManager::selectAll, actionCollection);

    createAction(Action::ExportAsSVG, tr("Export as SVG"), {QKeySequence(QKeyCombination(Qt::CTRL | Qt::SHIFT, Qt::Key_E))}, this, &ActionManager::exportToSvg)
        ->setIcon(QIcon::fromTheme(u"document-export"_s));
    createAction(Action::GroupItems, tr("Group Items"), {QKeySequence(QKeyCombination(Qt::CTRL, Qt::Key_G))}, this, &ActionManager::groupItems);
    createAction(Action::UngroupItems,
                 tr("Ungroup Items"),
                 {QKeySequence(QKeyCombination(Qt::CTRL | Qt::SHIFT, Qt::Key_G))},
                 this,
                 &ActionManager::ungroupItems);
    createAction(Action::DeleteSelection, tr("Delete"), {QKeySequence::Delete}, this, &ActionManager::deleteSelection);
    createAction(Action::Clear, tr("Clear Canvas"), {}, this, &ActionManager::clear)->setIcon(QIcon::fromTheme(u"edit-delete"_s));

    createToolAction(Action::SwitchToFreeformTool,
                     tr("Freeform Tool"),
                     {QKeySequence(QKeyCombination(Qt::Key_P)), QKeySequence(QKeyCombination(Qt::Key_B))},
                     Tool::Type::Freeform);
    createToolAction(Action::SwitchToEraserTool, tr("Eraser Tool"), {QKeySequence(QKeyCombination(Qt::Key_E))}, Tool::Type::Eraser);
    createToolAction(Action::SwitchToSelectionTool, tr("Selection Tool"), {QKeySequence(QKeyCombination(Qt::Key_S))}, Tool::Type::Selection);
    createToolAction(Action::SwitchToRectangleTool, tr("Rectangle Tool"), {QKeySequence(QKeyCombination(Qt::Key_R))}, Tool::Type::Rectangle);
    createToolAction(Action::SwitchToEllipseTool, tr("Ellipse Tool"), {QKeySequence(QKeyCombination(Qt::Key_O))}, Tool::Type::Ellipse);
    createToolAction(Action::SwitchToLineTool, tr("Line Tool"), {QKeySequence(QKeyCombination(Qt::Key_L))}, Tool::Type::Line);
    createToolAction(Action::SwitchToTextTool, tr("Text Tool"), {QKeySequence(QKeyCombination(Qt::Key_T))}, Tool::Type::Text);
    createToolAction(Action::SwitchToArrowTool, tr("Arrow Tool"), {QKeySequence(QKeyCombination(Qt::Key_A))}, Tool::Type::Arrow);
    createToolAction(Action::SwitchToMoveTool, tr("Move Tool"), {QKeySequence(QKeyCombination(Qt::Key_M))}, Tool::Type::Move);

    createAction(Action::SendToBack, tr("Send To Back"), {}, this, [this]() {
        zorderMove(ItemUtils::ZorderMove::SendToBack);
    })->setIcon(QIcon::fromTheme(u"viewimage"_s)); // just a placeholder icon for now
    createAction(Action::SendBackward, tr("Send Backward"), {}, this, [this]() {
        zorderMove(ItemUtils::ZorderMove::SendBackward);
    })->setIcon(QIcon::fromTheme(u"viewimage"_s)); // just a placeholder icon for now
    createAction(Action::BringForward, tr("Bring Forward"), {}, this, [this]() {
        zorderMove(ItemUtils::ZorderMove::BringForward);
    })->setIcon(QIcon::fromTheme(u"viewimage"_s)); // just a placeholder icon for now
    createAction(Action::BringToFront, tr("Bring To Front"), {}, this, [this]() {
        zorderMove(ItemUtils::ZorderMove::BringToFront);
    })->setIcon(QIcon::fromTheme(u"viewimage"_s)); // just a placeholder icon for now

    actionCollection->associateWidget(mainWindow);
    actionCollection->readSettings();

    // managing actions
    connect(m_context->spatialContext()->commandHistory(), &CommandHistory::undoRedoChanged, this, &ActionManager::slotUpdateHistoryButtons);
    connect(m_context->renderingContext(), &RenderingContext::zoomFactorChanged, this, &ActionManager::slotUpdateZoomButtons);
    connect(m_context->selectionContext(), &SelectionContext::selectionUpdated, this, &ActionManager::slotUpdateZorderButtons);

    connect(action(Action::BringForward), &QAction::triggered, this, &ActionManager::slotUpdateZorderButtons);
    connect(action(Action::SendBackward), &QAction::triggered, this, &ActionManager::slotUpdateZorderButtons);
    connect(action(Action::BringToFront), &QAction::triggered, this, &ActionManager::slotUpdateZorderButtons);
    connect(action(Action::SendToBack), &QAction::triggered, this, &ActionManager::slotUpdateZorderButtons);
}

QAction *ActionManager::action(Action type) const
{
    return m_actionCollection->action(actionName(type));
}

QString ActionManager::actionName(Action type) const
{
    switch (type) {
    case Action::GroupItems:
        return u"group_items"_s;
    case Action::UngroupItems:
        return u"ungroup_items"_s;
    case Action::DeleteSelection:
        return u"delete_selection"_s;
    case Action::ExportAsSVG:
        return u"export_as_svg"_s;
    case Action::SwitchToSelectionTool:
        return u"switch_to_selection_tool"_s;
    case Action::SwitchToFreeformTool:
        return u"switch_to_freeform_tool"_s;
    case Action::SwitchToRectangleTool:
        return u"switch_to_rectangle_tool"_s;
    case Action::SwitchToEllipseTool:
        return u"switch_to_ellipse_tool"_s;
    case Action::SwitchToLineTool:
        return u"switch_to_line_tool"_s;
    case Action::SwitchToArrowTool:
        return u"switch_to_arrow_tool"_s;
    case Action::SwitchToTextTool:
        return u"switch_to_text_tool"_s;
    case Action::SwitchToEraserTool:
        return u"switch_to_eraser_tool"_s;
    case Action::SwitchToMoveTool:
        return u"switch_to_move_tool"_s;
    case Action::Clear:
        return u"clear"_s;
    case Action::SendToBack:
        return u"send_to_back"_s;
    case Action::SendBackward:
        return u"send_backward"_s;
    case Action::BringForward:
        return u"bring_forward"_s;
    case Action::BringToFront:
        return u"bring_to_front"_s;
    }
    Q_UNREACHABLE();
    return u""_s;
}

QAction *ActionManager::action(KStandardActions::StandardAction standardAction) const
{
    return m_actionCollection->action(actionName(standardAction));
}

QString ActionManager::actionName(KStandardActions::StandardAction standardAction) const
{
    return KStandardActions::name(standardAction);
}

void ActionManager::undo()
{
    m_context->spatialContext()->commandHistory()->undo();
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::redo()
{
    m_context->spatialContext()->commandHistory()->redo();
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::zoomIn()
{
    m_context->renderingContext()->zoomIn();
}

void ActionManager::zoomOut()
{
    m_context->renderingContext()->zoomOut();
}

void ActionManager::switchToTool(Tool::Type type)
{
    m_context->uiContext()->toolBar()->changeTool(type);
}

void ActionManager::zorderMove(ItemUtils::ZorderMove move)
{
    auto &selectedItems{m_context->selectionContext()->selectedItems()};
    if (selectedItems.empty()) {
        return;
    }

    const QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<ZorderCommand>(items, move));
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::alignItems(ItemUtils::AlignType alignType)
{
    auto &selectedItems{m_context->selectionContext()->selectedItems()};
    if (selectedItems.empty()) {
        return;
    }
    const QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<AlignItemCommand>(items, alignType));
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::switchToMoveTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Type::Move);
}

void ActionManager::groupItems()
{
    auto &selectedItems{m_context->selectionContext()->selectedItems()};
    if (selectedItems.empty()) {
        return;
    }

    const QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<GroupCommand>(m_context, items));
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::ungroupItems()
{
    auto &selectedItems{m_context->selectionContext()->selectedItems()};
    if (selectedItems.empty()) {
        return;
    }

    const QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<UngroupCommand>(items));
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::deleteSelection()
{
    auto &selectedItems{m_context->selectionContext()->selectedItems()};
    auto commandHistory{m_context->spatialContext()->commandHistory()};

    const QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    commandHistory->insert(std::make_shared<RemoveItemCommand>(items));

    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();

    const QList<std::shared_ptr<Item>> selectedItemsVector{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<DeselectCommand>(selectedItemsVector));
}

void ActionManager::selectAll()
{
    switchToTool(Tool::Type::Selection);

    const auto allItems{m_context->spatialContext()->quadtree().getAllItems()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<SelectCommand>(allItems));

    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::newFile()
{
    if (!confirmSaveAfterModification()) {
        return;
    }

    m_context->reset();
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
    m_context->setCurrentFileModified(false);
}

void ActionManager::saveAsNewFile()
{
    const QDir homeDir{QDir::home()};
    QString text{tr("Untitled.%1").arg(Common::drawyFileExt)};
    const QString defaultFilePath = homeDir.filePath(text);
    text = QObject::tr("Drawy (*.%1)").arg(Common::drawyFileExt);
    const QString fileName{QFileDialog::getSaveFileName(nullptr, QObject::tr("Save File"), defaultFilePath, text)};
    if (fileName.isEmpty()) {
        return;
    }
    auto job = new SaveAsJob(m_context, this);
    const SaveAsJob::SaveAsInfo info{
        .filePath = fileName,
        .offsetPos = m_context->spatialContext()->offsetPos(),
        .zoomFactor = m_context->renderingContext()->zoomFactor(),
        .items = m_context->spatialContext()->quadtree().getAllItems(),
    };
    job->setSaveAsInfo(info);
    connect(job, &SaveAsJob::saveFileDone, this, [fileName](const QJsonObject &obj) {
        SerializerUtils::saveInFile(obj, fileName);
    });
    job->start();
}

void ActionManager::clear()
{
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(nullptr,
                                           tr("Do you really want to clear the canvas?"),
                                           tr("Clear"),
                                           KStandardGuiItem::ok(),
                                           KStandardGuiItem::cancel())) {
        const auto allItems{m_context->spatialContext()->quadtree().getAllItems()};
        m_context->spatialContext()->commandHistory()->insert(std::make_shared<RemoveItemCommand>(allItems));
        m_context->renderingContext()->markForRender();
        m_context->renderingContext()->markForUpdate();
    }
}

void ActionManager::saveCurrentFile()
{
    if (m_context->fileNeedsName()) {
        saveAsNewFile();
        return;
    }

    const auto fileName{m_context->currentFileName()};
    auto job = new SaveAsJob(m_context, this);
    const SaveAsJob::SaveAsInfo info{
        .filePath = fileName,
        .offsetPos = m_context->spatialContext()->offsetPos(),
        .zoomFactor = m_context->renderingContext()->zoomFactor(),
        .items = m_context->spatialContext()->quadtree().getAllItems(),
    };

    job->setSaveAsInfo(info);
    connect(job, &SaveAsJob::saveFileDone, this, [fileName](const QJsonObject &obj) {
        SerializerUtils::saveInFile(obj, fileName);
    });

    job->start();
}

void ActionManager::openFile()
{
    if (!confirmSaveAfterModification()) {
        return;
    }

    const QString filter = QObject::tr("Drawy (*.%1)").arg(Common::drawyFileExt);

    const QDir homeDir{QDir::home()};
    const QString fileName = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open File"), homeDir.path(), filter);
    if (fileName.isEmpty()) {
        return;
    }

    loadFile(fileName);
}

void ActionManager::exportToSvg()
{
    const QDir homeDir{QDir::home()};
    QString text = QObject::tr("Untitled.svg");
    const QString defaultFilePath = homeDir.filePath(text);
    text = QObject::tr("SVG (*.svg)");

    const QString fileName{QFileDialog::getSaveFileName(nullptr, QObject::tr("Export to Svg"), defaultFilePath, text)};

    if (fileName.isEmpty()) {
        return;
    }

    QFile file{fileName};
    if (!file.open(QIODevice::WriteOnly)) {
        qCWarning(DRAWY_LOG) << "Error: There was an error opening the export file.";
        return;
    }

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);

    SvgSerializer::writeSvg(stream, m_context->spatialContext()->quadtree().getAllItems());
}

void ActionManager::loadFile(const QString &fileName)
{
    auto job = new LoadJob(m_context, this);
    job->setFileName(fileName);
    connect(job, &LoadJob::loadDone, this, &ActionManager::slotLoadDone);
    job->start();
}

// The return value indicates whether you can proceed or not (if cancelled)
bool ActionManager::confirmSaveAfterModification()
{
    if (!m_context->currentFileModified()) {
        // continue with the action
        return true;
    }

    const auto fileName{m_context->currentFileName()};
    const auto message{tr("The document \"%1\" has been modified. Do you want to save your changes or discard them?").arg(fileName)};

    const int choice = KMessageBox::questionTwoActionsCancel(m_context->parentWidget(),
                                                             message,
                                                             tr("Action Required"),
                                                             KStandardGuiItem::save(),
                                                             KStandardGuiItem::discard(),
                                                             KStandardGuiItem::cancel());

    if (choice == KMessageBox::Cancel) {
        return false;
    }

    if (choice == KMessageBox::PrimaryAction) {
        saveCurrentFile();
    }

    return true;
}

void ActionManager::slotLoadDone(const LoadJob::LoadInfo &info)
{
    LoadJobUtil::loadFile(m_context, info);
}

QAction *ActionManager::createAction(const Action &type, const QString &title, const QList<QKeySequence> &keys)
{
    auto result{new QAction(title, m_actionCollection)};
    result->setShortcuts(keys);

    m_actionCollection->addAction(actionName(type), result);
    m_actionCollection->setDefaultShortcuts(result, keys);

    return result;
}

QAction *ActionManager::createToolAction(const Action &actionType, const QString &title, const QList<QKeySequence> &keys, Tool::Type toolType)
{
    return createAction(actionType, title, keys, this, [this, toolType]() -> void {
        switchToTool(toolType);
    });
}

void ActionManager::configureSettings()
{
    ConfigureSettingsDialog dlg(m_context->parentWidget());
    dlg.exec();
}

void ActionManager::slotUpdateZoomButtons()
{
    if (m_context->renderingContext()->canZoomIn()) {
        action(KStandardActions::ZoomIn)->setEnabled(true);
    } else {
        action(KStandardActions::ZoomIn)->setEnabled(false);
    }

    if (m_context->renderingContext()->canZoomOut()) {
        action(KStandardActions::ZoomOut)->setEnabled(true);
    } else {
        action(KStandardActions::ZoomOut)->setEnabled(false);
    }
}

void ActionManager::slotUpdateHistoryButtons()
{
    if (m_context->spatialContext()->commandHistory()->hasUndo()) {
        action(KStandardActions::Undo)->setEnabled(true);
    } else {
        action(KStandardActions::Undo)->setEnabled(false);
    }

    if (m_context->spatialContext()->commandHistory()->hasRedo()) {
        action(KStandardActions::Redo)->setEnabled(true);
    } else {
        action(KStandardActions::Redo)->setEnabled(false);
    }
}

void ActionManager::slotUpdateZorderButtons()
{
    auto selectionContext{m_context->selectionContext()};
    auto &quadtree{m_context->spatialContext()->quadtree()};

    bool atLeastOneCanMoveBack{false}, atLeastOneCanMoveForward{false};

    const auto selectedItems{selectionContext->selectedItems()};
    for (const auto &item : std::as_const(selectedItems)) {
        if (quadtree.canMoveBackwards(item)) {
            atLeastOneCanMoveBack = true;
        }

        if (quadtree.canMoveForwards(item)) {
            atLeastOneCanMoveForward = true;
        }
    }

    if (atLeastOneCanMoveForward) {
        action(Action::BringForward)->setEnabled(true);
        action(Action::BringToFront)->setEnabled(true);
    } else {
        action(Action::BringForward)->setEnabled(false);
        action(Action::BringToFront)->setEnabled(false);
    }

    if (atLeastOneCanMoveBack) {
        action(Action::SendBackward)->setEnabled(true);
        action(Action::SendToBack)->setEnabled(true);
    } else {
        action(Action::SendBackward)->setEnabled(false);
        action(Action::SendToBack)->setEnabled(false);
    }
}

#include "moc_actionmanager.cpp"
