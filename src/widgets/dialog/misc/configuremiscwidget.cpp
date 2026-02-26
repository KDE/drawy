/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "configuremiscwidget.hpp"
#include "dialog/misc/powermanagerwidget.hpp"
#include "drawyglobalconfig.h"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ConfigureMiscWidget::ConfigureMiscWidget(QWidget *parent)
    : QWidget{parent}
    , mPowerManagerWidget(new PowerManagerWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->addWidget(mPowerManagerWidget);
    mainLayout->addStretch(1);
}

ConfigureMiscWidget::~ConfigureMiscWidget() = default;

void ConfigureMiscWidget::save()
{
    mPowerManagerWidget->save();
    DrawyGlobalConfig::self()->save();
}

void ConfigureMiscWidget::load()
{
    mPowerManagerWidget->load();
}

void ConfigureMiscWidget::restoreToDefaults()
{
    const bool bUseDefaults = DrawyGlobalConfig::self()->useDefaults(true);
    mPowerManagerWidget->load();
    DrawyGlobalConfig::self()->useDefaults(bUseDefaults);
}

#include "moc_configuremiscwidget.cpp"
