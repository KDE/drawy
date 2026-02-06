/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <KHelpMenu>
class HelpMenu : public KHelpMenu
{
    Q_OBJECT
public:
    explicit HelpMenu(QWidget *parent = nullptr);
    ~HelpMenu() override;
};
