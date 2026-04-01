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
#include "drawyglobalconfig.h"
#include "event/event.hpp"
#include "iconmanager/iconmanager.hpp"
#include "keybindings/actionmanager.hpp"
#include "keybindings/keybindmanager.hpp"
#include "pluginform/pluginformmanager.hpp"
#include "properties/widgets/propertymanager.hpp"
#include "renderingcontext.hpp"
#include "selectioncontext.hpp"
#include "spatialcontext.hpp"
#include "tools/arrowtool.hpp"
#include "tools/customtool.hpp"
#include "tools/diamondtool.hpp"
#include "tools/ellipsetool.hpp"
#include "tools/erasertool.hpp"
#include "tools/freeformtool.hpp"
#include "tools/imagetool.hpp"
#include "tools/linetool.hpp"
#include "tools/movetool.hpp"
#include "tools/rectangletool.hpp"
#include "tools/selectiontool/selectiontool.hpp"
#include "tools/texttool.hpp"
#include <KLocalizedString>
#include <QMenu>

UIContext::UIContext(ApplicationContext *context)
    : QObject{context}
    , m_keybindManager(new KeybindManager(this))
    , m_event(new Event())
    , mPluginFormManager(PluginFormManager::self())
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
    m_actionManager = new ActionManager(m_keybindManager->actionCollection(), m_applicationContext);
    m_propertyBar = new PropertyBar(m_applicationContext, m_applicationContext->parentWidget());

    m_topWidgets = new TopWidgets(m_applicationContext, m_applicationContext->parentWidget());
    m_toolBar = new ToolBar(m_topWidgets);
    m_toolBar->addTool(std::make_shared<SelectionTool>(m_applicationContext), Tool::Type::Selection, i18nc("@action:button", "Selection"));
    m_toolBar->addTool(std::make_shared<FreeformTool>(m_applicationContext), Tool::Type::Freeform, i18nc("@action:button", "Free Form"));
    m_toolBar->addTool(std::make_shared<RectangleTool>(m_applicationContext), Tool::Type::Rectangle, i18nc("@action:button", "Rectangle"));
    m_toolBar->addTool(std::make_shared<DiamondTool>(m_applicationContext), Tool::Type::Diamond, i18nc("@action:button", "Diamond"));
    m_toolBar->addTool(std::make_shared<EllipseTool>(m_applicationContext), Tool::Type::Ellipse, i18nc("@action:button", "Ellipse"));
    m_toolBar->addTool(std::make_shared<ArrowTool>(m_applicationContext), Tool::Type::Arrow, i18nc("@action:button", "Arrow"));
    m_toolBar->addTool(std::make_shared<LineTool>(m_applicationContext), Tool::Type::Line, i18nc("@action:button", "Line"));
    m_toolBar->addTool(std::make_shared<EraserTool>(m_applicationContext), Tool::Type::Eraser, i18nc("@action:button", "Eraser"));
    m_toolBar->addTool(std::make_shared<TextTool>(m_applicationContext), Tool::Type::Text, i18nc("@action:button", "Text"));
    m_toolBar->addTool(std::make_shared<MoveTool>(m_applicationContext), Tool::Type::Move, i18nc("@action:button", "Move"));
    m_toolBar->addImageTool(std::make_shared<ImageTool>(m_applicationContext));
    m_toolBar->addCustomTool(std::make_shared<CustomTool>(m_applicationContext));

    connect(m_toolBar, &ToolBar::toolChanged, this, &UIContext::toolChanged);
    connect(m_toolBar, &ToolBar::toolbarShown, this, [this]() {
        if (!m_topWidgets->isInitialized()) {
            m_topWidgets->initialize();
        }
    });
    m_bottomLeftWidgets = new BottomLeftWidgets(m_actionManager, m_applicationContext->parentWidget());
    connect(m_bottomLeftWidgets, &BottomLeftWidgets::resetZoom, this, [this]() {
        m_applicationContext->renderingContext()->updateZoomFactor(1);
    });
    connect(m_applicationContext->renderingContext(), &RenderingContext::zoomFactorChanged, m_bottomLeftWidgets, &BottomLeftWidgets::zoomFactorChanged);
    m_topLeftWidgets = new TopLeftWidgets(m_actionManager, m_applicationContext->parentWidget());

    m_propertyManager = new PropertyManager(m_actionManager, m_propertyBar);
    m_propertyBar->setPropertyManager(m_propertyManager);
    connect(m_toolBar, &ToolBar::toolChanged, m_propertyBar, &PropertyBar::updateProperties);
    connect(m_propertyManager, &PropertyManager::propertyUpdated, m_applicationContext->selectionContext(), &SelectionContext::updatePropertyOfSelectedItems);
    connect(m_propertyManager, &PropertyManager::propertyUpdated, m_propertyBar, &PropertyBar::updateToolProperties);
    connect(m_applicationContext->selectionContext(), &SelectionContext::selectionUpdated, m_propertyBar, &PropertyBar::updateToolProperties);
    connect(m_applicationContext->renderingContext()->canvas(), &Canvas::customContextMenuRequested, this, &UIContext::showContextMenu);

    connect(this, &UIContext::themeChanged, &IconManager::instance(), &IconManager::slotUpdateIcons);
    connect(&IconManager::instance(), &IconManager::requestIconUpdate, this, [this]() -> void {
        IconManager::instance().slotUpdateIcons(isDarkTheme());
    });

    connect(DrawyGlobalConfig::self(), &DrawyGlobalConfig::configChanged, this, &UIContext::slotThemeChanged);
    slotThemeChanged();

    m_propertyBar->updateProperties(m_toolBar->curTool());
}

bool UIContext::isDarkTheme() const
{
    const QColor bgColor = QGuiApplication::palette().color(QPalette::Window);
    return bgColor.lightnessF() < 0.5;
}

void UIContext::slotThemeChanged()
{
    const bool isDark = isDarkTheme();
    const auto bgColor = isDark ? DrawyGlobalConfig::backgroundColorDark() : DrawyGlobalConfig::backgroundColorLight();
    m_applicationContext->renderingContext()->setCanvasBackground(bgColor);
    Q_EMIT themeChanged(isDark);
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
    canvas->setFocus(Qt::OtherFocusReason);

    m_applicationContext->renderingContext()->markForUpdate();
}

void UIContext::reset()
{
    m_lastTool = nullptr;
    toolBar()->changeTool(Tool::Type::Selection);
}

void UIContext::showContextMenu() const
{
    auto allItems{m_applicationContext->spatialContext()->quadtree().getAllItems()};
    bool hasItems = !allItems.empty();
    bool hasSelection = false;

    if (hasItems) {
        const auto &selectedItems{m_applicationContext->selectionContext()->selectedItems()};
        hasSelection = !selectedItems.empty();
    }

    auto menu = new QMenu(m_applicationContext->parentWidget());

    if (hasSelection) {
        menu->addAction(actionManager()->action(KStandardActions::Copy));
    }

    menu->addAction(actionManager()->action(KStandardActions::Paste));
    if (hasSelection) {
        menu->addAction(actionManager()->action(ActionManager::Action::DeleteSelection));
    }

    if (hasItems) {
        menu->addAction(actionManager()->action(KStandardActions::SelectAll));
    }

    menu->addSeparator();

    menu->addAction(actionManager()->action(KStandardActions::FullScreen));
    menu->addAction(actionManager()->action(ActionManager::Action::ExportAsImage));

    menu->addSeparator();

    menu->addAction(actionManager()->action(KStandardActions::Preferences));

    if (m_applicationContext->debug()) {
        menu->addAction(actionManager()->action(ActionManager::Action::Debug));
    }

    menu->addSeparator();

    menu->addAction(actionManager()->action(KStandardActions::Quit));

    menu->exec(QCursor::pos());
    delete menu;
}

#include "moc_uicontext.cpp"
