/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "helpmenu.hpp"

HelpMenu::HelpMenu(QWidget *parent)
    : KHelpMenu(parent)
{
}

HelpMenu::~HelpMenu() = default;

#include "moc_helpmenu.cpp"
