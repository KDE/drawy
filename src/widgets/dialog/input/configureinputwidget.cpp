/*
 * SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "configureinputwidget.hpp"
#include "drawyglobalconfig.h"
#include "freehandsettingswidget.hpp"
#include "selectionsettingswidget.hpp"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ConfigureInputWidget::ConfigureInputWidget(QWidget *parent)
    : QWidget{parent}
    , m_freehandSettingsWidget{new FreehandSettingsWidget(this)}
    , m_selectionSettingsWidget{new SelectionSettingsWidget(this)}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->addWidget(m_freehandSettingsWidget);
    mainLayout->addWidget(m_selectionSettingsWidget);
    mainLayout->addStretch(1);
}

ConfigureInputWidget::~ConfigureInputWidget() = default;

void ConfigureInputWidget::save()
{
    m_freehandSettingsWidget->save();
    m_selectionSettingsWidget->save();
    DrawyGlobalConfig::self()->save();
}

void ConfigureInputWidget::load()
{
    m_freehandSettingsWidget->load();
    m_selectionSettingsWidget->load();
}

void ConfigureInputWidget::restoreToDefaults()
{
    const bool bUseDefaults = DrawyGlobalConfig::self()->useDefaults(true);
    m_freehandSettingsWidget->load();
    m_selectionSettingsWidget->load();
    DrawyGlobalConfig::self()->useDefaults(bUseDefaults);
}

#include "moc_configureinputwidget.cpp"
