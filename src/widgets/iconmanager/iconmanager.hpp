// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

class QAction;
class QAbstractButton;

class IconManager : public QObject
{
    Q_OBJECT
public:
    static IconManager &instance();
    explicit IconManager();

    void setIcon(QAction *action, const QString &iconName);
    void setIcon(QAbstractButton *action, const QString &iconName);
    void slotUpdateIcons(bool isDarkTheme);

Q_SIGNALS:
    void requestIconUpdate();

private:
    QHash<QAction *, QString> m_actionMap;
    QHash<QAbstractButton *, QString> m_buttonMap;
};
