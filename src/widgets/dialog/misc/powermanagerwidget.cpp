/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "powermanagerwidget.hpp"
#include "drawyglobalconfig.h"
#include <KLocalizedString>
#include <QCheckBox>

PowerManagerWidget::PowerManagerWidget(QWidget *parent)
    : ConfigGroupInterface{parent, i18n("Power Manager")}
    , mEnabled{new QCheckBox(i18n("Disable screenlocker during full screen"), this)}
{
    m_layout->addWidget(mEnabled);
}

PowerManagerWidget::~PowerManagerWidget() = default;

void PowerManagerWidget::save()
{
    DrawyGlobalConfig::self()->setEnabledInFullScreen(mEnabled->isChecked());
    DrawyGlobalConfig::self()->save();
}

void PowerManagerWidget::load()
{
    mEnabled->setChecked(DrawyGlobalConfig::self()->enabledInFullScreen());
}

#include "moc_powermanagerwidget.cpp"
