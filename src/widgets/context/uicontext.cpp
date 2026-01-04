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
#include "drawy_debug.h"
#include "event/event.hpp"
#include "keybindings/actionmanager.hpp"
#include "keybindings/keybindmanager.hpp"
#include "properties/widgets/propertymanager.hpp"
#include "renderingcontext.hpp"
#include "selectioncontext.hpp"
#include "serializer/loader.hpp"
#include "serializer/serializer.hpp"
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
    m_keybindManager = new KeybindManager(m_applicationContext->renderingContext()->canvas());
    m_actionManager = new ActionManager(m_applicationContext);
    m_iconManager = new IconManager(m_applicationContext);

    m_propertyManager = new PropertyManager(m_propertyBar);
    m_propertyBar->setPropertyManager(m_propertyManager);

    connect(m_propertyManager, &PropertyManager::propertyUpdated, m_applicationContext->selectionContext(), &SelectionContext::updatePropertyOfSelectedItems);

    m_event = new Event();

    m_toolBar->addTool(std::make_shared<SelectionTool>(), Tool::Type::Selection);
    m_toolBar->addTool(std::make_shared<FreeformTool>(), Tool::Type::Freeform);
    m_toolBar->addTool(std::make_shared<RectangleTool>(), Tool::Type::Rectangle);
    m_toolBar->addTool(std::make_shared<EllipseTool>(), Tool::Type::Ellipse);
    m_toolBar->addTool(std::make_shared<ArrowTool>(), Tool::Type::Arrow);
    m_toolBar->addTool(std::make_shared<LineTool>(), Tool::Type::Line);
    m_toolBar->addTool(std::make_shared<EraserTool>(), Tool::Type::Eraser);
    m_toolBar->addTool(std::make_shared<TextTool>(), Tool::Type::Text);
    m_toolBar->addTool(std::make_shared<MoveTool>(), Tool::Type::Move);

    // TODO: Define their functions somewhere else
    m_actionBar->addButton(tr("Save to File"), IconManager::Icon::ACTION_SAVE);
    m_actionBar->addButton(tr("Open File"), IconManager::Icon::ACTION_OPEN_FILE);
    m_actionBar->addButton(tr("Zoom Out"), IconManager::Icon::ACTION_ZOOM_OUT);
    m_actionBar->addButton(tr("Zoom In"), IconManager::Icon::ACTION_ZOOM_IN);
    m_actionBar->addButton(tr("Light Mode"), IconManager::Icon::ACTION_LIGHT_MODE);
    m_actionBar->addButton(tr("Undo"), IconManager::Icon::ACTION_UNDO);
    m_actionBar->addButton(tr("Redo"), IconManager::Icon::ACTION_REDO);

    connect(m_toolBar, &ToolBar::toolChanged, this, &UIContext::toolChanged);
    connect(m_toolBar, &ToolBar::toolChanged, m_propertyBar, &PropertyBar::updateProperties);

    connect(m_actionBar->button(IconManager::Icon::ACTION_ZOOM_OUT), &QPushButton::clicked, this, [this]() {
        m_applicationContext->renderingContext()->updateZoomFactor(-1);
    });

    connect(m_actionBar->button(IconManager::Icon::ACTION_ZOOM_IN), &QPushButton::clicked, this, [this]() {
        m_applicationContext->renderingContext()->updateZoomFactor(1);
    });

    connect(m_actionBar->button(IconManager::Icon::ACTION_UNDO), &QPushButton::clicked, this, [this]() {
        m_applicationContext->spatialContext()->commandHistory()->undo();
        m_applicationContext->renderingContext()->markForRender();
        m_applicationContext->renderingContext()->markForUpdate();
    });

    connect(m_actionBar->button(IconManager::Icon::ACTION_REDO), &QPushButton::clicked, this, [this]() {
        m_applicationContext->spatialContext()->commandHistory()->redo();
        m_applicationContext->renderingContext()->markForRender();
        m_applicationContext->renderingContext()->markForUpdate();
    });

    connect(m_actionBar->button(IconManager::Icon::ACTION_SAVE), &QPushButton::clicked, this, [this]() {
        Serializer serializer{};

        serializer.serialize(m_applicationContext);
        serializer.saveToFile();
    });

    connect(m_actionBar->button(IconManager::Icon::ACTION_OPEN_FILE), &QPushButton::clicked, this, [this]() {
        Loader loader{};

        loader.loadFromFile(m_applicationContext);
    });

    connect(m_actionBar->button(IconManager::Icon::ACTION_LIGHT_MODE), &QPushButton::clicked, this, [this]() {
        Canvas *canvas{m_applicationContext->renderingContext()->canvas()};
        QPushButton *button{actionBar()->button(IconManager::Icon::ACTION_LIGHT_MODE)};

        if (canvas->bg() == Common::lightBackgroundColor) {
            canvas->setBg(Common::darkBackgroundColor);
            button->setToolTip(tr("Light Mode"));
            button->setIcon(iconManager()->icon(IconManager::Icon::ACTION_LIGHT_MODE));
        } else {
            canvas->setBg(Common::lightBackgroundColor);
            button->setToolTip(tr("Dark Mode"));
            button->setIcon(iconManager()->icon(IconManager::Icon::ACTION_DARK_MODE));
        }

        m_applicationContext->renderingContext()->markForRender();
        m_applicationContext->renderingContext()->markForUpdate();
    });

    m_propertyBar->updateProperties(m_toolBar->curTool());
}

ToolBar *UIContext::toolBar() const
{
    return m_toolBar;
}

PropertyBar *UIContext::propertyBar() const
{
    return m_propertyBar;
}

ActionBar *UIContext::actionBar() const
{
    return m_actionBar;
}

KeybindManager *UIContext::keybindManager() const
{
    return m_keybindManager;
}

ActionManager *UIContext::actionManager() const
{
    return m_actionManager;
}

PropertyManager *UIContext::propertyManager() const
{
    return m_propertyManager;
}

Event *UIContext::event() const
{
    return m_event;
}

IconManager *UIContext::iconManager() const
{
    return m_iconManager;
}

void UIContext::toolChanged(Tool *tool)
{
    if (tool->type() != Tool::Type::Selection) {
        m_applicationContext->selectionContext()->selectedItems().clear();
    }

    Common::renderCanvas(m_applicationContext);

    Canvas *canvas{m_applicationContext->renderingContext()->canvas()};

    if (m_lastTool != nullptr)
        m_lastTool->cleanup();

    m_lastTool = tool;
    canvas->setCursor(tool->cursor());

    m_applicationContext->renderingContext()->markForUpdate();
}

void UIContext::reset()
{
    m_lastTool = nullptr;
    toolBar()->changeTool(Tool::Type::Selection);
}

#include "moc_uicontext.cpp"
