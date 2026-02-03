// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "uicontext.hpp"

#include "applicationcontext.hpp"
#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "common/constants.hpp"
#include "common/renderitems.hpp"
#include "components/bottomleftwidgets.hpp"
#include "components/propertybar.hpp"
#include "components/toolbar.hpp"
#include "components/topleftwidgets.hpp"
#include "components/topwidgets.hpp"
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
#include "tools/diamondtool.hpp"
#include "tools/ellipsetool.hpp"
#include "tools/erasertool.hpp"
#include "tools/freeformtool.hpp"
#include "tools/linetool.hpp"
#include "tools/movetool.hpp"
#include "tools/rectangletool.hpp"
#include "tools/selectiontool/selectiontool.hpp"
#include "tools/texttool.hpp"
#include <QMenu>

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

void UIContext::initializeUIContext()
{
    m_keybindManager = new KeybindManager(m_applicationContext->renderingContext()->canvas());
    m_actionManager = new ActionManager(m_applicationContext);
    m_topWidgets = new TopWidgets(m_applicationContext->parentWidget());
    m_toolBar = new ToolBar(m_topWidgets);
    m_bottomLeftWidgets = new BottomLeftWidgets(m_applicationContext->parentWidget());
    m_topLeftWidgets = new TopLeftWidgets(m_applicationContext->parentWidget());
    m_propertyBar = new PropertyBar(m_applicationContext->parentWidget());

    m_propertyManager = new PropertyManager(m_propertyBar);
    m_propertyBar->setPropertyManager(m_propertyManager);

    connect(m_propertyManager, &PropertyManager::propertyUpdated, m_applicationContext->selectionContext(), &SelectionContext::updatePropertyOfSelectedItems);

    m_event = new Event();

    m_toolBar->addTool(std::make_shared<SelectionTool>(), Tool::Type::Selection, tr("Selection"));
    m_toolBar->addTool(std::make_shared<FreeformTool>(), Tool::Type::Freeform, tr("Free Form"));
    m_toolBar->addTool(std::make_shared<RectangleTool>(), Tool::Type::Rectangle, tr("Rectangle"));
    m_toolBar->addTool(std::make_shared<DiamondTool>(), Tool::Type::Diamond, tr("Diamond"));
    m_toolBar->addTool(std::make_shared<EllipseTool>(), Tool::Type::Ellipse, tr("Ellipse"));
    m_toolBar->addTool(std::make_shared<ArrowTool>(), Tool::Type::Arrow, tr("Arrow"));
    m_toolBar->addTool(std::make_shared<LineTool>(), Tool::Type::Line, tr("Line"));
    m_toolBar->addTool(std::make_shared<EraserTool>(), Tool::Type::Eraser, tr("Eraser"));
    m_toolBar->addTool(std::make_shared<TextTool>(), Tool::Type::Text, tr("Text"));
    m_toolBar->addTool(std::make_shared<MoveTool>(), Tool::Type::Move, tr("Move"));

    connect(m_toolBar, &ToolBar::toolChanged, this, &UIContext::toolChanged);
    connect(m_toolBar, &ToolBar::toolChanged, m_propertyBar, &PropertyBar::updateProperties);
    connect(m_toolBar, &ToolBar::toolbarShown, this, [this]() {
        if (!m_topWidgets->isInitialized()) {
            m_topWidgets->initialize();
        }
    });

    connect(m_applicationContext->selectionContext(), &SelectionContext::selectionUpdated, m_propertyBar, &PropertyBar::updateToolProperties);

    connect(m_applicationContext->renderingContext()->canvas(), &Canvas::customContextMenuRequested, this, &UIContext::showContextMenu);

    m_propertyBar->updateProperties(m_toolBar->curTool());
}

ToolBar *UIContext::toolBar() const
{
    return m_toolBar;
}

TopWidgets *UIContext::topWidgets() const
{
    return m_topWidgets;
}

PropertyBar *UIContext::propertyBar() const
{
    return m_propertyBar;
}

BottomLeftWidgets *UIContext::bottomLeftWidgets() const
{
    return m_bottomLeftWidgets;
}

TopLeftWidgets *UIContext::topLeftWidgets() const
{
    return m_topLeftWidgets;
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

Event *UIContext::appEvent() const
{
    return m_event;
}

void UIContext::toolChanged(Tool &tool)
{
    if (tool.type() != Tool::Type::Selection) {
        m_applicationContext->selectionContext()->reset();
    }

    Common::renderCanvas(m_applicationContext);

    auto canvas{m_applicationContext->renderingContext()->canvas()};

    if (m_lastTool != nullptr) {
        m_lastTool->cleanup();
    }

    m_lastTool = &tool;
    canvas->setCursor(tool.cursor());

    m_applicationContext->renderingContext()->markForUpdate();
}

void UIContext::reset()
{
    m_lastTool = nullptr;
    toolBar()->changeTool(Tool::Type::Selection);
}

void UIContext::showContextMenu() const
{
    auto context{ApplicationContext::instance()};
    auto allItems{context->spatialContext()->quadtree().getAllItems()};

    auto menu = new QMenu(context->parentWidget());
    menu->addAction(actionManager()->action(KStandardActions::FullScreen));
    menu->addSeparator();
    if (!allItems.empty()) {
        menu->addAction(actionManager()->action(KStandardActions::SelectAll));
        menu->addSeparator();
    }
    menu->addAction(actionManager()->action(ActionManager::Action::ExportAsSVG));
    menu->addSeparator();
    menu->addAction(actionManager()->action(KStandardActions::Preferences));
    menu->addSeparator();
    menu->addAction(actionManager()->action(KStandardActions::Quit));
    menu->exec(QCursor::pos());
    delete menu;
}

#include "moc_uicontext.cpp"
