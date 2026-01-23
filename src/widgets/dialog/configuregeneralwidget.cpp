/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "configuregeneralwidget.hpp"
#include "drawyglobalconfig.h"
#include <QLabel>
#include <QVBoxLayout>
#include <qspinbox.h>

using namespace Qt::Literals::StringLiterals;
ConfigureGeneralWidget::ConfigureGeneralWidget(QWidget *parent)
    : QWidget{parent}
    , mAutoSave(new QSpinBox(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mAutoSave->setObjectName(u"mAutoSave"_s);
    mainLayout->addWidget(mAutoSave);
    mainLayout->addStretch(0);
}

ConfigureGeneralWidget::~ConfigureGeneralWidget() = default;

void ConfigureGeneralWidget::save()
{
    DrawyGlobalConfig::self()->setDelay(mAutoSave->value());
}

void ConfigureGeneralWidget::load()
{
    mAutoSave->setValue(DrawyGlobalConfig::self()->delay());
}

void ConfigureGeneralWidget::restoreToDefaults()
{
    const bool bUseDefaults = DrawyGlobalConfig::self()->useDefaults(true);
    mAutoSave->setValue(DrawyGlobalConfig::self()->delay());
    DrawyGlobalConfig::self()->useDefaults(bUseDefaults);
}

#include "moc_configuregeneralwidget.cpp"
