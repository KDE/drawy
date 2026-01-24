// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "actionmanager.hpp"

#include <QDir>
#include <QFileDialog>
#include <memory>

#include "action.hpp"
#include "command/commandhistory.hpp"
#include "command/deselectcommand.hpp"
#include "command/groupcommand.hpp"
#include "command/removeitemcommand.hpp"
#include "command/selectcommand.hpp"
#include "command/ungroupcommand.hpp"
#include "common/constants.hpp"
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
#include "jobs/loadjobutil.hpp"
#include "jobs/saveasjob.hpp"
#include "keybindmanager.hpp"
#include "serializer/serializerutils.hpp"
using namespace Qt::Literals::StringLiterals;
ActionManager::ActionManager(ApplicationContext *context)
    : QObject(context)
    , m_context{context}
{
    auto keybindManager{m_context->uiContext()->keybindManager()};

    // NOLINT(clang-analyzer-cplusplus.NewDeleteLeaks)
    auto freeformToolAction{new Action{tr("Freeform Tool"),
                                       tr("Switch to freeform drawing tool"),
                                       [this]() {
                                           switchToFreeformTool();
                                       },
                                       this}};

    auto eraserToolAction{new Action{tr("Eraser Tool"),
                                     tr("Switch to eraser tool"),
                                     [this]() {
                                         switchToEraserTool();
                                     },
                                     this}};

    auto selectionToolAction{new Action{tr("Selection Tool"),
                                        tr("Switch to selection tool"),
                                        [&]() {
                                            switchToSelectionTool();
                                        },
                                        this}};

    auto rectangleToolAction{new Action{tr("Rectangle Tool"),
                                        tr("Switch to rectangle drawing tool"),
                                        [this]() {
                                            switchToRectangleTool();
                                        },
                                        this}};

    auto ellipseToolAction{new Action{tr("Ellipse Tool"),
                                      tr("Switch to ellipse drawing tool"),
                                      [this]() {
                                          switchToEllipseTool();
                                      },
                                      this}};

    auto lineToolAction{new Action{tr("Line Tool"),
                                   tr("Switch to line drawing tool"),
                                   [&]() {
                                       switchToLineTool();
                                   },
                                   this}};

    auto textToolAction{new Action{tr("Text Tool"),
                                   tr("Switch to the text tool"),
                                   [this]() {
                                       switchToTextTool();
                                   },
                                   this}};

    auto arrowToolAction{new Action{tr("Arrow Tool"),
                                    tr("Switch to arrow drawing tool"),
                                    [this]() {
                                        switchToArrowTool();
                                    },
                                    this}};

    auto moveToolAction{new Action{tr("Move Tool"),
                                   tr("Switch to move tool"),
                                   [this]() {
                                       switchToMoveTool();
                                   },
                                   this}};

    auto groupAction{new Action{tr("Group Elements"),
                                tr("Groups selected items"),
                                [this]() {
                                    groupItems();
                                },
                                this}};

    auto unGroupAction{new Action{tr("Ungroup Elements"),
                                  tr("Ungroups selected groups"),
                                  [this]() {
                                      ungroupItems();
                                  },
                                  this}};

    auto deleteAction{new Action{tr("Delete"),
                                 tr("Deletes selected items"),
                                 [this]() {
                                     deleteSelection();
                                 },
                                 this}};

    keybindManager->addKeybinding(textToolAction, QKeySequence(QKeyCombination(Qt::Key_T)));
    keybindManager->addKeybinding(freeformToolAction, QKeySequence(QKeyCombination(Qt::Key_P)));
    keybindManager->addKeybinding(freeformToolAction, QKeySequence(QKeyCombination(Qt::Key_B)));
    keybindManager->addKeybinding(eraserToolAction, QKeySequence(QKeyCombination(Qt::Key_E)));
    keybindManager->addKeybinding(selectionToolAction, QKeySequence(QKeyCombination(Qt::Key_S)));
    keybindManager->addKeybinding(rectangleToolAction, QKeySequence(QKeyCombination(Qt::Key_R)));
    keybindManager->addKeybinding(ellipseToolAction, QKeySequence(QKeyCombination(Qt::Key_O)));
    keybindManager->addKeybinding(lineToolAction, QKeySequence(QKeyCombination(Qt::Key_L)));
    keybindManager->addKeybinding(arrowToolAction, QKeySequence(QKeyCombination(Qt::Key_A)));
    keybindManager->addKeybinding(moveToolAction, QKeySequence(QKeyCombination(Qt::Key_M)));
    keybindManager->addKeybinding(deleteAction, QKeySequence(QKeySequence::Delete));
    keybindManager->addKeybinding(groupAction, QKeySequence(QKeyCombination(Qt::Key_G)));
    keybindManager->addKeybinding(unGroupAction, QKeySequence(QKeyCombination(Qt::CTRL | Qt::SHIFT, Qt::Key_G)));
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
    m_context->renderingContext()->updateZoomFactor(1);
}

void ActionManager::zoomOut()
{
    m_context->renderingContext()->updateZoomFactor(-1);
}

void ActionManager::switchToFreeformTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Type::Freeform);
}

void ActionManager::switchToEraserTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Type::Eraser);
}

void ActionManager::switchToRectangleTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Type::Rectangle);
}

void ActionManager::switchToEllipseTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Type::Ellipse);
}

void ActionManager::switchToLineTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Type::Line);
}

void ActionManager::switchToArrowTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Type::Arrow);
}

void ActionManager::switchToMoveTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Type::Move);
}

void ActionManager::switchToSelectionTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Type::Selection);
}

void ActionManager::switchToTextTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Type::Text);
}

void ActionManager::groupItems()
{
    auto &selectedItems{m_context->selectionContext()->selectedItems()};
    if (selectedItems.size() <= 1)
        return;

    QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<GroupCommand>(items));
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::ungroupItems()
{
    auto &selectedItems{m_context->selectionContext()->selectedItems()};
    if (selectedItems.empty())
        return;

    QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<UngroupCommand>(items));
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::deleteSelection()
{
    auto &selectedItems{m_context->selectionContext()->selectedItems()};
    auto commandHistory{m_context->spatialContext()->commandHistory()};

    QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    commandHistory->insert(std::make_shared<RemoveItemCommand>(items));

    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();

    QList<std::shared_ptr<Item>> selectedItemsVector{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<DeselectCommand>(selectedItemsVector));
}

void ActionManager::selectAll()
{
    switchToSelectionTool();

    auto allItems{m_context->spatialContext()->quadtree().getAllItems()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<SelectCommand>(allItems));

    m_context->uiContext()->propertyBar()->updateToolProperties();
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::saveToFile()
{
    const QDir homeDir{QDir::home()};
    QString text = QObject::tr("Untitled.%1").arg(Common::drawyFileExt);
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
        qDebug() << " save done ";
    });
    job->start();
}

void ActionManager::loadFromFile()
{
    const QString filter = QObject::tr("Drawy (*.%1)").arg(Common::drawyFileExt);

    // ask for file (handle cancel)
    const QDir homeDir{QDir::home()};
    const QString fileName = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open File"), homeDir.path(), filter);
    if (fileName.isEmpty())
        return;
    loadFile(fileName);
}

void ActionManager::loadFile(const QString &fileName)
{
    auto job = new LoadJob(this);
    job->setFileName(fileName);
    m_context->setCurrentFileName(fileName);
    connect(job, &LoadJob::loadDone, this, &ActionManager::slotLoadDone);
    job->start();
}

void ActionManager::slotLoadDone(const LoadJob::LoadInfo &info)
{
    LoadJobUtil::loadFile(info);
}
#include "moc_actionmanager.cpp"
