/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "toolbuttonplugin.hpp"

ToolButtonPlugin::ToolButtonPlugin(QWidget *parent)
    : QToolButton(parent)
{
    setCheckable(true);
    setChecked(false);
    setAutoRaise(true);
}

ToolButtonPlugin::~ToolButtonPlugin() = default;

#include "moc_toolbuttonplugin.cpp"
