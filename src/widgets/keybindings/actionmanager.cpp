// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "actionmanager.hpp"

#include <KActionCollection>
#include <KGuiItem>
#include <KMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QMenu>
#include <QXmlStreamWriter>

#include "command/alignitemcommand.hpp"
#include "command/commandhistory.hpp"
#include "command/deselectcommand.hpp"
#include "command/groupcommand.hpp"
#include "command/removeitemcommand.hpp"
#include "command/selectcommand.hpp"
#include "command/ungroupcommand.hpp"
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
ActionManager::ActionManager(ApplicationContext *context)
    : QObject(context)
    , m_context{context}
{
    auto actionCollection{context->uiContext()->keybindManager()->actionCollection()};
    auto mainWindow{context->parentWidget()};

    KStandardActions::preferences(this, &ActionManager::configureSettings, actionCollection);
    KStandardActions::openNew(this, &ActionManager::newFile, actionCollection);
    KStandardActions::save(this, &ActionManager::saveCurrentFile, actionCollection);
    KStandardActions::saveAs(this, &ActionManager::saveAsNewFile, actionCollection);
    KStandardActions::open(this, &ActionManager::openFile, actionCollection);
    KStandardActions::undo(this, &ActionManager::undo, actionCollection);
    KStandardActions::redo(this, &ActionManager::redo, actionCollection);
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

    actionCollection->associateWidget(mainWindow);
    actionCollection->readSettings();

    // managing actions
    connect(context->spatialContext()->commandHistory(), &CommandHistory::undoRedoChanged, this, [context, this]() -> void {
        if (context->spatialContext()->commandHistory()->hasUndo()) {
            action(KStandardActions::Undo)->setEnabled(true);
        } else {
            action(KStandardActions::Undo)->setEnabled(false);
        }

        if (context->spatialContext()->commandHistory()->hasRedo()) {
            action(KStandardActions::Redo)->setEnabled(true);
        } else {
            action(KStandardActions::Redo)->setEnabled(false);
        }
    });

    connect(context->renderingContext(), &RenderingContext::zoomFactorChanged, this, [context, this]([[maybe_unused]] qreal newZoomFactor) {
        if (context->renderingContext()->canZoomIn()) {
            action(KStandardActions::ZoomIn)->setEnabled(true);
        } else {
            action(KStandardActions::ZoomIn)->setEnabled(false);
        }

        if (context->renderingContext()->canZoomOut()) {
            action(KStandardActions::ZoomOut)->setEnabled(true);
        } else {
            action(KStandardActions::ZoomOut)->setEnabled(false);
        }
    });
}

QAction *ActionManager::action(Action type) const
{
    auto actionManager{m_context->uiContext()->keybindManager()->actionCollection()};
    return actionManager->action(actionName(type));
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
    }

    return u""_s;
}

QAction *ActionManager::action(KStandardActions::StandardAction standardAction) const
{
    auto actionManager{m_context->uiContext()->keybindManager()->actionCollection()};
    return actionManager->action(actionName(standardAction));
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
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<GroupCommand>(items));
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

    auto allItems{m_context->spatialContext()->quadtree().getAllItems()};
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
    auto job = new SaveAsJob(this);
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
        == KMessageBox::questionTwoActions(nullptr, tr("Do you want to clear canvas?"), tr("Clear"), KStandardGuiItem::ok(), KStandardGuiItem::cancel())) {
        ApplicationContext *context{ApplicationContext::instance()};
        context->reset();
        context->renderingContext()->cacheGrid().markAllDirty();
        context->renderingContext()->markForRender();
        context->renderingContext()->markForUpdate();
    }
}

void ActionManager::saveCurrentFile()
{
    auto fileName{m_context->currentFileName()};
    if (m_context->fileNeedsName()) {
        saveAsNewFile();
        return;
    }

    auto job = new SaveAsJob(this);
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
    auto job = new LoadJob(this);
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
    LoadJobUtil::loadFile(info);
}

QAction *ActionManager::createAction(const Action &type, const QString &title, const QList<QKeySequence> &keys)
{
    auto context{ApplicationContext::instance()};
    auto actionCollection{context->uiContext()->keybindManager()->actionCollection()};
    auto result{new QAction(title, actionCollection)};
    result->setShortcuts(keys);

    actionCollection->addAction(actionName(type), result);
    actionCollection->setDefaultShortcuts(result, keys);

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

void ActionManager::showHamburgerMenu()
{
}

#include "moc_actionmanager.cpp"
