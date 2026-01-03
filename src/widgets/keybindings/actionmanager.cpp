// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "actionmanager.hpp"

#include <memory>

#include "action.hpp"
#include "command/commandhistory.hpp"
#include "command/deselectcommand.hpp"
#include "command/groupcommand.hpp"
#include "command/removeitemcommand.hpp"
#include "command/selectcommand.hpp"
#include "command/ungroupcommand.hpp"
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
#include "keybindmanager.hpp"
#include "serializer/loader.hpp"
#include "serializer/serializer.hpp"
using namespace Qt::Literals::StringLiterals;
ActionManager::ActionManager(ApplicationContext *context)
    : QObject(context)
    , m_context{context}
{
    KeybindManager *keybindManager{m_context->uiContext()->keybindManager()};

    // NOLINT(clang-analyzer-cplusplus.NewDeleteLeaks)
    Action *undoAction{new Action{tr("Undo"),
                                  tr("Undo last action"),
                                  [&]() {
                                      this->undo();
                                  },
                                  context}};

    Action *redoAction{new Action{tr("Redo"),
                                  tr("Redo last undone action"),
                                  [&]() {
                                      this->redo();
                                  },
                                  context}};

    Action *zoomInAction{new Action{tr("Zoom In"),
                                    tr("Zoom in"),
                                    [&]() {
                                        this->zoomIn();
                                    },
                                    context}};

    Action *zoomOutAction{new Action{tr("Zoom Out"),
                                     tr("Zoom out"),
                                     [&]() {
                                         this->zoomOut();
                                     },
                                     context}};

    Action *freeformToolAction{new Action{tr("Freeform Tool"),
                                          tr("Switch to freeform drawing tool"),
                                          [&]() {
                                              this->switchToFreeformTool();
                                          },
                                          context}};

    Action *eraserToolAction{new Action{tr("Eraser Tool"),
                                        tr("Switch to eraser tool"),
                                        [&]() {
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
                                           [&]() {
                                               this->switchToRectangleTool();
                                           },
                                           context}};

    Action *ellipseToolAction{new Action{tr("Ellipse Tool"),
                                         tr("Switch to ellipse drawing tool"),
                                         [&]() {
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
                                      [&]() {
                                          this->switchToTextTool();
                                      },
                                      context}};

    Action *arrowToolAction{new Action{tr("Arrow Tool"),
                                       tr("Switch to arrow drawing tool"),
                                       [&]() {
                                           this->switchToArrowTool();
                                       },
                                       context}};

    Action *moveToolAction{new Action{tr("Move Tool"),
                                      tr("Switch to move tool"),
                                      [&]() {
                                          this->switchToMoveTool();
                                      },
                                      context}};

    Action *groupAction{new Action{tr("Group Elements"),
                                   tr("Groups selected items"),
                                   [&]() {
                                       this->groupItems();
                                   },
                                   context}};

    Action *unGroupAction{new Action{tr("Ungroup Elements"),
                                     tr("Ungroups selected groups"),
                                     [&]() {
                                         this->ungroupItems();
                                     },
                                     context}};

    Action *selectAllAction{new Action{tr("Select All"),
                                       tr("Select all items"),
                                       [&]() {
                                           this->selectAll();
                                       },
                                       context}};

    Action *deleteAction{new Action{tr("Delete"),
                                    tr("Deletes selected items"),
                                    [&]() {
                                        this->deleteSelection();
                                    },
                                    context}};

    Action *saveAction{new Action{tr("Save"),
                                  tr("Save canvas"),
                                  [&]() {
                                      this->saveToFile();
                                  },
                                  context}};

    Action *openFileAction{new Action{tr("Open File"),
                                      tr("Open an existing file"),
                                      [&]() {
                                          this->loadFromFile();
                                      },
                                      context}};

    keybindManager->addKeybinding(undoAction, tr("Ctrl+Z"));
    keybindManager->addKeybinding(redoAction, tr("Ctrl+Y"));
    keybindManager->addKeybinding(redoAction, tr("Ctrl+Shift+Z"));
    keybindManager->addKeybinding(zoomInAction, tr("Ctrl++"));
    keybindManager->addKeybinding(zoomOutAction, tr("Ctrl+-"));
    keybindManager->addKeybinding(textToolAction, tr("T"));
    keybindManager->addKeybinding(freeformToolAction, tr("P"));
    keybindManager->addKeybinding(freeformToolAction, tr("B"));
    keybindManager->addKeybinding(eraserToolAction, tr("E"));
    keybindManager->addKeybinding(selectionToolAction, tr("S"));
    keybindManager->addKeybinding(rectangleToolAction, tr("R"));
    keybindManager->addKeybinding(ellipseToolAction, tr("O"));
    keybindManager->addKeybinding(lineToolAction, tr("L"));
    keybindManager->addKeybinding(arrowToolAction, tr("A"));
    keybindManager->addKeybinding(moveToolAction, tr("M"));
    keybindManager->addKeybinding(selectAllAction, tr("Ctrl+A"));
    keybindManager->addKeybinding(deleteAction, tr("Delete"));
    keybindManager->addKeybinding(saveAction, tr("Ctrl+S"));
    keybindManager->addKeybinding(openFileAction, tr("Ctrl+O"));
    keybindManager->addKeybinding(groupAction, tr("Ctrl+G"));
    keybindManager->addKeybinding(unGroupAction, tr("Ctrl+Shift+G"));
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
    m_context->uiContext()->toolBar()->changeTool(Tool::Freeform);
}

void ActionManager::switchToEraserTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Eraser);
}

void ActionManager::switchToRectangleTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Rectangle);
}

void ActionManager::switchToEllipseTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Ellipse);
}

void ActionManager::switchToLineTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Line);
}

void ActionManager::switchToArrowTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Arrow);
}

void ActionManager::switchToMoveTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Move);
}

void ActionManager::switchToSelectionTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Selection);
}

void ActionManager::switchToTextTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Text);
}

void ActionManager::groupItems()
{
    auto &selectedItems{m_context->selectionContext()->selectedItems()};
    if (selectedItems.size() <= 1)
        return;

    QVector<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<GroupCommand>(items));
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::ungroupItems()
{
    auto &selectedItems{m_context->selectionContext()->selectedItems()};
    if (selectedItems.empty())
        return;

    QVector<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<UngroupCommand>(items));
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::deleteSelection()
{
    auto selectedItems{m_context->selectionContext()->selectedItems()};
    auto commandHistory{m_context->spatialContext()->commandHistory()};

    QVector<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    commandHistory->insert(std::make_shared<RemoveItemCommand>(items));

    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();

    QVector<std::shared_ptr<Item>> selectedItemsVector{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<DeselectCommand>(selectedItemsVector));
}

void ActionManager::selectAll()
{
    this->switchToSelectionTool();

    auto allItems{m_context->spatialContext()->quadtree()->getAllItems()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<SelectCommand>(allItems));

    m_context->uiContext()->propertyBar()->updateToolProperties();
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::saveToFile()
{
    Serializer serializer{};

    serializer.serialize(m_context);
    serializer.saveToFile();
}

void ActionManager::loadFromFile()
{
    Loader loader{};
    loader.loadFromFile(m_context);
}

#include "moc_actionmanager.cpp"
