// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "iconmanager.hpp"
#include "context/uicontext.hpp"
#include <QAbstractButton>
using namespace Qt::StringLiterals;

IconManager::IconManager(ApplicationContext *context)
    : QObject{context}
    , m_context{context}
{
}

void IconManager::setIcon(QAction *action, const QString &iconName)
{
    m_actionMap[action] = iconName;

    slotUpdateIcons(m_context->uiContext()->isDarkTheme());
}

void IconManager::setIcon(QAbstractButton *button, const QString &iconName)
{
    m_buttonMap[button] = iconName;

    slotUpdateIcons(m_context->uiContext()->isDarkTheme());
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
