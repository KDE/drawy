/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "toolbuttonspluginwidget.hpp"
#include "iconmanager/iconmanager.hpp"
#include "toolbuttonplugin.hpp"
#include "toolbuttonselectplugin.hpp"
#include "tools/customtool.hpp"
#include "tools/tool.hpp"

ToolButtonsPluginWidget::ToolButtonsPluginWidget(QWidget *parentWidget, QObject *parent)
    : QObject{parent}
    , mToolButtonSelectPlugin(new ToolButtonSelectPlugin(parentWidget))
    , mToolButtonPlugin(new ToolButtonPlugin(parentWidget))
{
    mToolButtonSelectPlugin->setFocusPolicy(Qt::NoFocus);
    mToolButtonPlugin->setFocusPolicy(Qt::NoFocus);
    mToolButtonPlugin->setVisible(false);

    connect(mToolButtonSelectPlugin, &ToolButtonSelectPlugin::toolActivated, this, &ToolButtonsPluginWidget::slotToolActivated);
}

ToolButtonsPluginWidget::~ToolButtonsPluginWidget() = default;

ToolButtonSelectPlugin *ToolButtonsPluginWidget::toolButtonSelectPlugin() const
{
    return mToolButtonSelectPlugin;
}

ToolButtonPlugin *ToolButtonsPluginWidget::toolButtonPlugin() const
{
    return mToolButtonPlugin;
}

void ToolButtonsPluginWidget::slotToolActivated(const PluginForm::PluginFormInfo &item)
{
    if (!mToolButtonPlugin->isVisible()) {
        mToolButtonPlugin->setVisible(true);
    }
    mTool->setUpdateTool(item);
    mToolButtonPlugin->setToolTip(item.toolTip);
    mToolButtonPlugin->setChecked(true);

    if (item.useCustomIcon) {
        IconManager::instance().setIcon(mToolButtonPlugin, item.iconName);
    } else {
        mToolButtonPlugin->setIcon(QIcon::fromTheme(item.iconName));
    }

    Q_EMIT toolChanged(*mTool);
}

void ToolButtonsPluginWidget::setTool(const std::shared_ptr<CustomTool> &newTool)
{
    mTool = newTool;
}
#include "moc_toolbuttonspluginwidget.cpp"
