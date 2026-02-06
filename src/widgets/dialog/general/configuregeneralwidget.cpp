/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "configuregeneralwidget.hpp"
#include "autosavewidget.hpp"
#include "drawyglobalconfig.h"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ConfigureGeneralWidget::ConfigureGeneralWidget(QWidget *parent)
    : QWidget{parent}
    , m_autoSaveWidget{new AutoSaveWidget{this}}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mainLayout->addWidget(m_autoSaveWidget);
    mainLayout->addStretch(0);

    m_autoSaveWidget->setObjectName(u"m_autoSaveWidget"_s);
}

ConfigureGeneralWidget::~ConfigureGeneralWidget() = default;

void ConfigureGeneralWidget::save()
{
    m_autoSaveWidget->save();
    DrawyGlobalConfig::self()->save();
}

void ConfigureGeneralWidget::load()
{
    m_autoSaveWidget->load();
}

void ConfigureGeneralWidget::restoreToDefaults()
{
    const bool bUseDefaults = DrawyGlobalConfig::self()->useDefaults(true);
    m_autoSaveWidget->load();
    DrawyGlobalConfig::self()->useDefaults(bUseDefaults);
}

#include "moc_configuregeneralwidget.cpp"
