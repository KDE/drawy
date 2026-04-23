// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QHash>
#include <QObject>
class QAction;
class QAbstractButton;
class QString;

class IconManager : public QObject
{
    Q_OBJECT
public:
    static IconManager &instance();
    explicit IconManager();

    void setIcon(QAction *action, const QString &iconName);
    void setIcon(QAbstractButton *button, const QString &iconName);
    void slotUpdateIcons(bool isDarkTheme);

Q_SIGNALS:
    void requestIconUpdate();

private:
    QHash<QAction *, QString> m_actionMap;
    QHash<QAbstractButton *, QString> m_buttonMap;
};
