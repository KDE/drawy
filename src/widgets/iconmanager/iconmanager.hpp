// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "context/applicationcontext.hpp"
class QAction;
class QAbstractButton;

class IconManager : public QObject
{
public:
    explicit IconManager(ApplicationContext *context);

    void setIcon(QAction *action, const QString &iconName);
    void setIcon(QAbstractButton *action, const QString &iconName);
    void slotUpdateIcons(bool isDarkTheme);

private:
    ApplicationContext *const m_context;
    QHash<QAction *, QString> m_actionMap;
    QHash<QAbstractButton *, QString> m_buttonMap;
};
