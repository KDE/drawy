// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QHBoxLayout>
#include <QWidget>
class HelpMenu;
class KActionMenu;
class ActionManager;
class TopLeftWidgets : public QWidget
{
    Q_OBJECT
public:
    explicit TopLeftWidgets(ActionManager *actionManager, QWidget *parent = nullptr);

private:
    QHBoxLayout *const m_layout;
    HelpMenu *const mHelpMenu;
    KActionMenu *const mColorSchemeMenu;
};
