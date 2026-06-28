// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "toolbar.hpp"
#include "components/toolbuttonplugin.hpp"
#include "components/toolbuttonselectplugin.hpp"
#include "components/toolbuttonspluginwidget.hpp"
#include "iconmanager/iconmanager.hpp"
#include <KLocalizedString>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QStyle>
#include <QToolButton>

using namespace Qt::Literals::StringLiterals;
ToolBar::ToolBar(QWidget *parent)
    : Frame{parent}
    , m_group(new QButtonGroup(this))
    , m_layout(new QHBoxLayout(this))
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_layout->setSpacing(style()->pixelMetric(QStyle::PM_LayoutHorizontalSpacing));
    connect(m_group, &QButtonGroup::idClicked, this, &ToolBar::onToolChanged);
}

ToolBar::~ToolBar() = default;

Tool &ToolBar::curTool() const
{
    const Tool::Type curID{static_cast<Tool::Type>(m_group->checkedId())};

    if (!m_tools.contains(curID)) {
        throw std::logic_error("Trying to access a non existent tool");
    }

    return *m_tools.at(curID);
}

void ToolBar::addCustomTool(const std::shared_ptr<CustomTool> &tool)
{
    if (!tool) {
        return;
    }

    auto buttonsPluginWidget = new ToolButtonsPluginWidget(this, this);
    m_tools[Tool::Type::Custom] = tool;
    buttonsPluginWidget->setTool(tool);
    m_group->addButton(buttonsPluginWidget->toolButtonPlugin(), static_cast<int>(Tool::Type::Custom));
    m_layout->addWidget(buttonsPluginWidget->toolButtonPlugin());
    m_layout->addWidget(buttonsPluginWidget->toolButtonSelectPlugin());
    connect(buttonsPluginWidget, &ToolButtonsPluginWidget::toolChanged, this, &ToolBar::toolChanged);
}

void ToolBar::addImageTool(const std::shared_ptr<Tool> &tool)
{
    if (!tool) {
        return;
    }

    auto btn = new QToolButton(this);
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setToolTip(i18nc("@action:button", "Image"));
    btn->setIcon(QIcon::fromTheme(tool->icon()));
    btn->setAutoRaise(true);
    btn->setCheckable(true);

    connect(btn, &QToolButton::clicked, this, [this, tool, btn]() {
        btn->setChecked(true);
        Q_EMIT toolChanged(*tool);
    });

    const int iconSize{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    btn->setIconSize(QSize{iconSize, iconSize});

    btn->setCursor(Qt::PointingHandCursor);
    m_tools[Tool::Type::Image] = tool;
    m_group->addButton(btn, static_cast<int>(Tool::Type::Image));
    m_layout->addWidget(btn);
}

void ToolBar::addTool(const std::shared_ptr<Tool> &tool, Tool::Type type, const QString &name)
{
    if (!tool) {
        return;
    }

    if (type == Tool::Type::Custom || type == Tool::Type::Image) {
        return;
    }

    auto btn = new QToolButton(this);
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setToolTip(name);
    btn->setAutoRaise(true);

    if (tool->useCustomIcon()) {
        IconManager::instance().setIcon(btn, tool->icon());
    } else {
        btn->setIcon(QIcon::fromTheme(tool->icon()));
    }

    const int iconSize{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    btn->setIconSize(QSize{iconSize, iconSize});

    btn->setCheckable(true);
    btn->setCursor(Qt::PointingHandCursor);

    m_tools[type] = tool;
    m_group->addButton(btn, static_cast<int>(type));
    m_layout->addWidget(btn);
    if (m_tools.size() == 1) {
        btn->setChecked(true);
        Q_EMIT toolChanged(*tool);
    }
}

void ToolBar::changeTool(Tool::Type type)
{
    const int id{static_cast<int>(type)};

    m_group->button(id)->setChecked(true);
    Q_EMIT toolChanged(curTool());
}

Tool &ToolBar::tool(Tool::Type type) const
{
    return *m_tools.at(type);
}

void ToolBar::onToolChanged([[maybe_unused]] int id)
{
    Q_EMIT toolChanged(curTool());
}

void ToolBar::showEvent([[maybe_unused]] QShowEvent *event)
{
    Q_EMIT toolbarShown();
}

#include "moc_toolbar.cpp"
