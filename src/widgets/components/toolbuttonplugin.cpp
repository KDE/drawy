/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "toolbuttonplugin.hpp"
#include <QStyle>

ToolButtonPlugin::ToolButtonPlugin(QWidget *parent)
    : QToolButton(parent)
{
    setCheckable(true);
    setChecked(false);
    setAutoRaise(true);
    setCursor(Qt::PointingHandCursor);
    const int iconSize{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    setIconSize(QSize{iconSize, iconSize});
}

ToolButtonPlugin::~ToolButtonPlugin() = default;

#include "moc_toolbuttonplugin.cpp"
