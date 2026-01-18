// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "uicontext.hpp"

#include "applicationcontext.hpp"
#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "common/constants.hpp"
#include "common/renderitems.hpp"
#include "components/actionbar.hpp"
#include "components/propertybar.hpp"
#include "components/toolbar.hpp"
#include "data-structures/quadtree.hpp"
#include "drawy_debug.h"
#include "event/event.hpp"
#include "keybindings/actionmanager.hpp"
#include "keybindings/keybindmanager.hpp"
#include "properties/widgets/propertymanager.hpp"
#include "renderingcontext.hpp"
#include "selectioncontext.hpp"
#include "spatialcontext.hpp"
#include "tools/arrowtool.hpp"
#include "tools/ellipsetool.hpp"
#include "tools/erasertool.hpp"
#include "tools/freeformtool.hpp"
#include "tools/linetool.hpp"
#include "tools/movetool.hpp"
#include "tools/rectangletool.hpp"
#include "tools/selectiontool/selectiontool.hpp"
#include "tools/texttool.hpp"

UIContext::UIContext(ApplicationContext *context)
    : QObject{context}
    , m_applicationContext{context}
{
}

UIContext::~UIContext()
{
    delete m_event;
    qCDebug(DRAWY_LOG) << "Object deleted: UIContext";
}

void UIContext::setUIContext()
{
    m_toolBar = new ToolBar(m_applicationContext->parentWidget());
    m_actionBar = new ActionBar(m_applicationContext->parentWidget());
    m_propertyBar = new PropertyBar(m_applicationContext->parentWidget());
    m_keybindManager = new KeybindManager(&m_applicationContext->renderingContext()->canvas());
    m_actionManager = new ActionManager(m_applicationContext);
    m_iconManager = new IconManager(m_applicationContext);

    m_propertyManager = new PropertyManager(m_propertyBar);
    m_propertyBar->setPropertyManager(m_propertyManager);

    connect(m_propertyManager, &PropertyManager::propertyUpdated, m_applicationContext->selectionContext(), &SelectionContext::updatePropertyOfSelectedItems);

    m_event = new Event();

    m_toolBar->addTool(std::make_shared<SelectionTool>(), Tool::Type::Selection, tr("Selection"));
    m_toolBar->addTool(std::make_shared<FreeformTool>(), Tool::Type::Freeform, tr("Free Form"));
    m_toolBar->addTool(std::make_shared<RectangleTool>(), Tool::Type::Rectangle, tr("Rectangle"));
    m_toolBar->addTool(std::make_shared<EllipseTool>(), Tool::Type::Ellipse, tr("Ellipse"));
    m_toolBar->addTool(std::make_shared<ArrowTool>(), Tool::Type::Arrow, tr("Arrow"));
    m_toolBar->addTool(std::make_shared<LineTool>(), Tool::Type::Line, tr("Line"));
    m_toolBar->addTool(std::make_shared<EraserTool>(), Tool::Type::Eraser, tr("Eraser"));
    m_toolBar->addTool(std::make_shared<TextTool>(), Tool::Type::Text, tr("Text"));
    m_toolBar->addTool(std::make_shared<MoveTool>(), Tool::Type::Move, tr("Move"));

    auto button = m_actionBar->addButton(tr("Save to File"), IconManager::Icon::ACTION_SAVE);
    connect(button, &QPushButton::clicked, this, [this]() {
        ActionManager &actionManager{m_applicationContext->uiContext()->actionManager()};
        actionManager.saveToFile();
    });

    button = m_actionBar->addButton(tr("Open File"), IconManager::Icon::ACTION_OPEN_FILE);
    connect(button, &QPushButton::clicked, this, [this]() {
        ActionManager &actionManager{m_applicationContext->uiContext()->actionManager()};
        actionManager.loadFromFile();
    });

    button = m_actionBar->addButton(tr("Zoom Out"), IconManager::Icon::ACTION_ZOOM_OUT);
    connect(button, &QPushButton::clicked, this, [this]() {
        m_applicationContext->renderingContext()->updateZoomFactor(-1);
    });

    button = m_actionBar->addButton(tr("Zoom In"), IconManager::Icon::ACTION_ZOOM_IN);
    connect(button, &QPushButton::clicked, this, [this]() {
        m_applicationContext->renderingContext()->updateZoomFactor(1);
    });
    button = m_actionBar->addButton(tr("Light Mode"), IconManager::Icon::ACTION_LIGHT_MODE);
    connect(button, &QPushButton::clicked, this, [this, button]() {
        auto &canvas{m_applicationContext->renderingContext()->canvas()};

        if (canvas.canvasBg() == Common::lightBackgroundColor) {
            canvas.setCanvasBg(Common::darkBackgroundColor);
            button->setToolTip(tr("Light Mode"));
            button->setIcon(iconManager()->icon(IconManager::Icon::ACTION_LIGHT_MODE));
        } else {
            canvas.setCanvasBg(Common::lightBackgroundColor);
            button->setToolTip(tr("Dark Mode"));
            button->setIcon(iconManager()->icon(IconManager::Icon::ACTION_DARK_MODE));
        }

        m_applicationContext->renderingContext()->markForRender();
        m_applicationContext->renderingContext()->markForUpdate();
    });

    auto undoButton = m_actionBar->addButton(tr("Undo"), IconManager::Icon::ACTION_UNDO);
    connect(undoButton, &QPushButton::clicked, this, [this]() {
        m_applicationContext->spatialContext()->commandHistory()->undo();
        m_applicationContext->renderingContext()->markForRender();
        m_applicationContext->renderingContext()->markForUpdate();
    });
    undoButton->setEnabled(false);
    connect(m_applicationContext->spatialContext()->commandHistory(), &CommandHistory::undoRedoChanged, this, [undoButton, this]() {
        undoButton->setEnabled(m_applicationContext->spatialContext()->commandHistory()->hasUndo());
    });

    auto redoButton = m_actionBar->addButton(tr("Redo"), IconManager::Icon::ACTION_REDO);
    connect(redoButton, &QPushButton::clicked, this, [this]() {
        m_applicationContext->spatialContext()->commandHistory()->redo();
        m_applicationContext->renderingContext()->markForRender();
        m_applicationContext->renderingContext()->markForUpdate();
    });
    redoButton->setEnabled(false);
    connect(m_applicationContext->spatialContext()->commandHistory(), &CommandHistory::undoRedoChanged, this, [redoButton, this]() {
        redoButton->setEnabled(m_applicationContext->spatialContext()->commandHistory()->hasRedo());
    });

    connect(m_toolBar, &ToolBar::toolChanged, this, &UIContext::toolChanged);
    connect(m_toolBar, &ToolBar::toolChanged, m_propertyBar, &PropertyBar::updateProperties);

    m_propertyBar->updateProperties(m_toolBar->curTool());
}

ToolBar &UIContext::toolBar() const
{
    return *m_toolBar;
}

PropertyBar &UIContext::propertyBar() const
{
    return *m_propertyBar;
}

ActionBar &UIContext::actionBar() const
{
    return *m_actionBar;
}

KeybindManager &UIContext::keybindManager() const
{
    return *m_keybindManager;
}

ActionManager &UIContext::actionManager() const
{
    return *m_actionManager;
}

PropertyManager &UIContext::propertyManager() const
{
    return *m_propertyManager;
}

Event &UIContext::appEvent() const
{
    return *m_event;
}

IconManager *UIContext::iconManager() const
{
    return m_iconManager;
}

void UIContext::toolChanged(Tool &tool)
{
    if (tool.type() != Tool::Type::Selection) {
        m_applicationContext->selectionContext()->selectedItems().clear();
    }

    Common::renderCanvas(m_applicationContext);

    auto &canvas{m_applicationContext->renderingContext()->canvas()};

    if (m_lastTool != nullptr)
        m_lastTool->cleanup();

    m_lastTool = &tool;
    canvas.setCursor(tool.cursor());

    m_applicationContext->renderingContext()->markForUpdate();
}

void UIContext::reset()
{
    m_lastTool = nullptr;
    toolBar().changeTool(Tool::Type::Selection);
}

#include "moc_uicontext.cpp"
