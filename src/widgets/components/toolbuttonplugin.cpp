/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "toolbuttonplugin.hpp"
#include "flowlayout.hpp"
#include "iconmanager/iconmanager.hpp"
#include "pluginform/pluginform.hpp"
#include "pluginform/pluginformmanager.hpp"
#include <KLocalizedString>
#include <KSeparator>
#include <QLabel>
#include <QMenu>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidgetAction>
using namespace Qt::StringLiterals;

ToolButtonPlugin::ToolButtonPlugin(QWidget *parent)
    : QToolButton(parent)
{
    setFocusPolicy(Qt::NoFocus);
    setPopupMode(QToolButton::InstantPopup);
    fillMenu();
    const int iconSize{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    setIconSize(QSize{iconSize, iconSize});
    setAutoRaise(true);
    setCheckable(true);
    setCursor(Qt::PointingHandCursor);
    setVisible(!PluginFormManager::self()->isEmpty());
    connect(this, &ToolButtonPlugin::toolActivated, this, &ToolButtonPlugin::updateToolButton);
    setToolTip(i18nc("@info:tooltip", "More Tools"));
    setIcon(QIcon::fromTheme(u"expand"_s));
}

ToolButtonPlugin::~ToolButtonPlugin() = default;

void ToolButtonPlugin::fillMenu()
{
    auto menu = new QMenu(this);
    auto pluginCustomWidget = new QWidget(menu);
    auto pluginCustomLayout = new QVBoxLayout(pluginCustomWidget);
    const auto pluginList = PluginFormManager::self()->pluginsList();
    const int iconSize{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    for (const auto &plugin : pluginList) {
        auto label = new QLabel(plugin->title(), pluginCustomWidget);
        QFont f = label->font();
        f.setBold(true);
        f.setItalic(true);
        label->setFont(f);
        pluginCustomLayout->addWidget(label);
        auto separator = new KSeparator(pluginCustomWidget);
        pluginCustomLayout->addWidget(separator);
        auto flowLayout = new FlowLayout;
        pluginCustomLayout->addLayout(flowLayout);
        for (const auto &item : plugin->pluginFormInfos()) {
            auto toolButton = new QToolButton(pluginCustomWidget);
            toolButton->setFocusPolicy(Qt::NoFocus);
            toolButton->setToolTip(item.toolTip);

            if (item.useCustomIcon) {
                IconManager::instance().setIcon(toolButton, item.iconName);
            } else {
                toolButton->setIcon(QIcon::fromTheme(item.iconName));
            }

            toolButton->setAutoRaise(true);
            toolButton->setIconSize(QSize{iconSize, iconSize});

            connect(toolButton, &QToolButton::clicked, this, [this, item]() {
                Q_EMIT toolActivated(item);
            });
            flowLayout->addWidget(toolButton);
        }
    }
    auto widgetAction = new QWidgetAction(menu);
    widgetAction->setDefaultWidget(pluginCustomWidget);

    menu->addAction(widgetAction);
    setMenu(menu);
}

void ToolButtonPlugin::updateToolButton([[maybe_unused]] const PluginForm::PluginFormInfo &item)
{
    setPopupMode(QToolButton::DelayedPopup);
}

#include "moc_toolbuttonplugin.cpp"
