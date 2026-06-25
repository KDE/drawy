/*
 * SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "configureinputwidget.hpp"
#include "drawyglobalconfig.h"
#include "freehandsettingswidget.hpp"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ConfigureInputWidget::ConfigureInputWidget(QWidget *parent)
    : QWidget{parent}
    , m_freehandSettingsWidget{new FreehandSettingsWidget(this)}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->addWidget(m_freehandSettingsWidget);
    mainLayout->addStretch(1);
}

ConfigureInputWidget::~ConfigureInputWidget() = default;

void ConfigureInputWidget::save()
{
    m_freehandSettingsWidget->save();
    DrawyGlobalConfig::self()->save();
}

void ConfigureInputWidget::load()
{
    m_freehandSettingsWidget->load();
}

void ConfigureInputWidget::restoreToDefaults()
{
    const bool bUseDefaults = DrawyGlobalConfig::self()->useDefaults(true);
    m_freehandSettingsWidget->load();
    DrawyGlobalConfig::self()->useDefaults(bUseDefaults);
}

#include "moc_configureinputwidget.cpp"
