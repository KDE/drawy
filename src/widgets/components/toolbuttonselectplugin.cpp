/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "toolbuttonselectplugin.hpp"
#include "flowlayout.hpp"
#include "iconmanager/iconmanager.hpp"
#include "pluginform/pluginformmanager.hpp"
#include <KLocalizedString>
#include <KSeparator>
#include <QLabel>
#include <QMenu>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidgetAction>

using namespace Qt::StringLiterals;

namespace
{

QLabel *createPluginHeaderLabel(const QString &title, QWidget *parent)
{
    auto label = new QLabel(title, parent);
    QFont font = label->font();
    font.setBold(true);
    font.setItalic(true);
    label->setFont(font);
    return label;
}

QToolButton *createPluginToolButton(const PluginForm::PluginFormInfo &item, int iconSize, QWidget *parent)
{
    auto toolButton = new QToolButton(parent);
    toolButton->setFocusPolicy(Qt::NoFocus);
    toolButton->setToolTip(item.toolTip);
    toolButton->setAutoRaise(true);
    toolButton->setIconSize(QSize{iconSize, iconSize});

    if (item.useCustomIcon) {
        IconManager::instance().setIcon(toolButton, item.iconName);
    } else {
        toolButton->setIcon(QIcon::fromTheme(item.iconName));
    }

    return toolButton;
}

}

ToolButtonSelectPlugin::ToolButtonSelectPlugin(QWidget *parent)
    : QToolButton(parent)
{
    setFocusPolicy(Qt::NoFocus);
    setAutoRaise(true);
    setCheckable(false);
    setCursor(Qt::PointingHandCursor);
    setPopupMode(QToolButton::InstantPopup);

    const int iconSize = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    setIconSize(QSize{iconSize, iconSize});
    setIcon(QIcon::fromTheme(u"expand"_s));
    setToolTip(i18nc("@info:tooltip", "More Tools"));

    setPopupMode(QToolButton::InstantPopup);
    fillMenu();

    setVisible(!PluginFormManager::self()->isEmpty());
}

ToolButtonSelectPlugin::~ToolButtonSelectPlugin() = default;

void ToolButtonSelectPlugin::fillMenu()
{
    auto menu = new QMenu(this);
    auto pluginCustomWidget = new QWidget(menu);
    auto pluginCustomLayout = new QVBoxLayout(pluginCustomWidget);

    const int iconSize = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    const auto pluginList = PluginFormManager::self()->pluginsList();

    for (const auto &plugin : pluginList) {
        pluginCustomLayout->addWidget(createPluginHeaderLabel(plugin->title(), pluginCustomWidget));
        pluginCustomLayout->addWidget(new KSeparator(pluginCustomWidget));

        auto flowLayout = new FlowLayout;
        pluginCustomLayout->addLayout(flowLayout);

        const auto &info = plugin->pluginFormInfos();
        for (const auto &item : std::as_const(info)) {
            auto toolButton = createPluginToolButton(item, iconSize, pluginCustomWidget);

            connect(toolButton, &QToolButton::clicked, this, [this, item, menu]() {
                Q_EMIT toolActivated(item);
                menu->close();
            });

            flowLayout->addWidget(toolButton);
        }
    }

    auto widgetAction = new QWidgetAction(menu);
    widgetAction->setDefaultWidget(pluginCustomWidget);

    menu->addAction(widgetAction);
    setMenu(menu);
}

#include "moc_toolbuttonselectplugin.cpp"
