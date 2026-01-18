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
#include "jobs/saveasjob.hpp"
#include "keybindmanager.hpp"
#include "serializer/serializerutils.hpp"
using namespace Qt::Literals::StringLiterals;
ActionManager::ActionManager(ApplicationContext *context)
    : QObject(context)
    , m_context{context}
{
    KeybindManager &keybindManager{m_context->uiContext()->keybindManager()};

    // NOLINT(clang-analyzer-cplusplus.NewDeleteLeaks)
    Action *undoAction{new Action{tr("Undo"),
                                  tr("Undo last action"),
                                  [this]() {
                                      this->undo();
                                  },
                                  context}};

    Action *redoAction{new Action{tr("Redo"),
                                  tr("Redo last undone action"),
                                  [this]() {
                                      this->redo();
                                  },
                                  context}};

    Action *zoomInAction{new Action{tr("Zoom In"),
                                    tr("Zoom in"),
                                    [this]() {
                                        this->zoomIn();
                                    },
                                    context}};

    Action *zoomOutAction{new Action{tr("Zoom Out"),
                                     tr("Zoom out"),
                                     [this]() {
                                         this->zoomOut();
                                     },
                                     context}};

    Action *freeformToolAction{new Action{tr("Freeform Tool"),
                                          tr("Switch to freeform drawing tool"),
                                          [this]() {
                                              this->switchToFreeformTool();
                                          },
                                          context}};

    Action *eraserToolAction{new Action{tr("Eraser Tool"),
                                        tr("Switch to eraser tool"),
                                        [this]() {
                                            this->switchToEraserTool();
                                        },
                                        context}};

    Action *selectionToolAction{new Action{tr("Selection Tool"),
                                           tr("Switch to selection tool"),
                                           [&]() {
                                               this->switchToSelectionTool();
                                           },
                                           context}};

    Action *rectangleToolAction{new Action{tr("Rectangle Tool"),
                                           tr("Switch to rectangle drawing tool"),
                                           [this]() {
                                               this->switchToRectangleTool();
                                           },
                                           context}};

    Action *ellipseToolAction{new Action{tr("Ellipse Tool"),
                                         tr("Switch to ellipse drawing tool"),
                                         [this]() {
                                             this->switchToEllipseTool();
                                         },
                                         context}};

    Action *lineToolAction{new Action{tr("Line Tool"),
                                      tr("Switch to line drawing tool"),
                                      [&]() {
                                          this->switchToLineTool();
                                      },
                                      context}};

    Action *textToolAction{new Action{tr("Text Tool"),
                                      tr("Switch to the text tool"),
                                      [this]() {
                                          this->switchToTextTool();
                                      },
                                      context}};

    Action *arrowToolAction{new Action{tr("Arrow Tool"),
                                       tr("Switch to arrow drawing tool"),
                                       [this]() {
                                           this->switchToArrowTool();
                                       },
                                       context}};

    Action *moveToolAction{new Action{tr("Move Tool"),
                                      tr("Switch to move tool"),
                                      [this]() {
                                          this->switchToMoveTool();
                                      },
                                      context}};

    Action *groupAction{new Action{tr("Group Elements"),
                                   tr("Groups selected items"),
                                   [this]() {
                                       this->groupItems();
                                   },
                                   context}};

    Action *unGroupAction{new Action{tr("Ungroup Elements"),
                                     tr("Ungroups selected groups"),
                                     [this]() {
                                         this->ungroupItems();
                                     },
                                     context}};

    Action *selectAllAction{new Action{tr("Select All"),
                                       tr("Select all items"),
                                       [this]() {
                                           this->selectAll();
                                       },
                                       context}};

    Action *deleteAction{new Action{tr("Delete"),
                                    tr("Deletes selected items"),
                                    [this]() {
                                        this->deleteSelection();
                                    },
                                    context}};

    Action *saveAction{new Action{tr("Save"),
                                  tr("Save canvas"),
                                  [this]() {
                                      this->saveToFile();
                                  },
                                  context}};

    Action *openFileAction{new Action{tr("Open File"),
                                      tr("Open an existing file"),
                                      [this]() {
                                          this->loadFromFile();
                                      },
                                      context}};

    keybindManager.addKeybinding(undoAction, tr("Ctrl+Z", "keybinding for the undo action"));
    keybindManager.addKeybinding(redoAction, tr("Ctrl+Y", "keybinding for the redo action"));
    keybindManager.addKeybinding(redoAction, tr("Ctrl+Shift+Z", "keybinding for the redo action"));
    keybindManager.addKeybinding(zoomInAction, tr("Ctrl++", "keybinding for the zoom in action"));
    keybindManager.addKeybinding(zoomOutAction, tr("Ctrl+-", "keybinding for the zoom out action"));
    keybindManager.addKeybinding(textToolAction, tr("T", "keybinding for the text tool"));
    keybindManager.addKeybinding(freeformToolAction, tr("P", "keybinding for the freeform drawing tool"));
    keybindManager.addKeybinding(freeformToolAction, tr("B", "keybinding for the freeform drawing tool"));
    keybindManager.addKeybinding(eraserToolAction, tr("E", "keybinding for the eraser tool"));
    keybindManager.addKeybinding(selectionToolAction, tr("S", "keybinding for the selection tool"));
    keybindManager.addKeybinding(rectangleToolAction, tr("R", "keybinding for the rectangle drawing tool"));
    keybindManager.addKeybinding(ellipseToolAction, tr("O", "keybinding for the ellipse drawing tool"));
    keybindManager.addKeybinding(lineToolAction, tr("L", "keybinding for the line drawing tool"));
    keybindManager.addKeybinding(arrowToolAction, tr("A", "keybinding for the arrow drawing tool"));
    keybindManager.addKeybinding(moveToolAction, tr("M", "keybinding for the move tool"));
    keybindManager.addKeybinding(selectAllAction, tr("Ctrl+A", "keybinding for the select all action"));
    keybindManager.addKeybinding(deleteAction, tr("Del", "keybinding for the delete action"));
    keybindManager.addKeybinding(saveAction, tr("Ctrl+S", "keybinding for the save action"));
    keybindManager.addKeybinding(openFileAction, tr("Ctrl+O", "keybinding for the open file action"));
    keybindManager.addKeybinding(groupAction, tr("Ctrl+G", "keybinding for the group action"));
    keybindManager.addKeybinding(unGroupAction, tr("Ctrl+Shift+G", "keybinding for the ungroup action"));
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
    m_context->uiContext()->toolBar().changeTool(Tool::Type::Freeform);
}

void ActionManager::switchToEraserTool()
{
    m_context->uiContext()->toolBar().changeTool(Tool::Type::Eraser);
}

void ActionManager::switchToRectangleTool()
{
    m_context->uiContext()->toolBar().changeTool(Tool::Type::Rectangle);
}

void ActionManager::switchToEllipseTool()
{
    m_context->uiContext()->toolBar().changeTool(Tool::Type::Ellipse);
}

void ActionManager::switchToLineTool()
{
    m_context->uiContext()->toolBar().changeTool(Tool::Type::Line);
}

void ActionManager::switchToArrowTool()
{
    m_context->uiContext()->toolBar().changeTool(Tool::Type::Arrow);
}

void ActionManager::switchToMoveTool()
{
    m_context->uiContext()->toolBar().changeTool(Tool::Type::Move);
}

void ActionManager::switchToSelectionTool()
{
    m_context->uiContext()->toolBar().changeTool(Tool::Type::Selection);
}

void ActionManager::switchToTextTool()
{
    m_context->uiContext()->toolBar().changeTool(Tool::Type::Text);
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
    this->switchToSelectionTool();

    auto allItems{m_context->spatialContext()->quadtree().getAllItems()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<SelectCommand>(allItems));

    m_context->uiContext()->propertyBar().updateToolProperties();
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
    connect(job, &LoadJob::loadDone, this, &ActionManager::slotLoadDone);
    job->start();
}

void ActionManager::slotLoadDone(const LoadJob::LoadInfo &info)
{
    ApplicationContext *context{ApplicationContext::instance()};
    context->reset();
    QuadTree &quadtree{context->spatialContext()->quadtree()};
    for (const auto &item : info.items) {
        quadtree.insertItem(item);
    }
    context->renderingContext()->setZoomFactor(info.zoomFactor);

    context->spatialContext()->setOffsetPos(info.offsetPos);
    context->renderingContext()->cacheGrid().markAllDirty();
    context->renderingContext()->markForRender();
    context->renderingContext()->markForUpdate();
}
#include "moc_actionmanager.cpp"
