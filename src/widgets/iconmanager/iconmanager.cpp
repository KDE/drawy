// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "iconmanager.hpp"
#include "context/uicontext.hpp"
#include <QAbstractButton>
using namespace Qt::StringLiterals;

IconManager::IconManager()
{
}

void IconManager::setIcon(QAction *action, const QString &iconName)
{
    m_actionMap[action] = iconName;

    Q_EMIT requestIconUpdate();
}

void IconManager::setIcon(QAbstractButton *button, const QString &iconName)
{
    m_buttonMap[button] = iconName;

    Q_EMIT requestIconUpdate();
}

IconManager &IconManager::instance()
{
    static IconManager iconManager;

    return iconManager;
}

void IconManager::slotUpdateIcons(bool isDarkTheme)
{
    const auto themeString{isDarkTheme ? u"dark/"_s : u"light/"_s};

    for (auto iterator{m_actionMap.begin()}; iterator != m_actionMap.end(); iterator++) {
        iterator.key()->setIcon(QIcon(u":/icons/"_s + themeString + iterator.value()));
    }

    for (auto iterator{m_buttonMap.begin()}; iterator != m_buttonMap.end(); iterator++) {
        iterator.key()->setIcon(QIcon(u":/icons/"_s + themeString + iterator.value()));
    }
}

#include "moc_iconmanager.cpp"
