/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "configuregeneralwidget.hpp"
#include "autosavewidget.hpp"
#include "canvascolorwidget.hpp"
#include "drawyglobalconfig.h"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ConfigureGeneralWidget::ConfigureGeneralWidget(QWidget *parent)
    : QWidget{parent}
    , m_autoSaveWidget{new AutoSaveWidget{this}}
    , m_canvasColorWidget{new CanvasColorWidget{this}}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mainLayout->addWidget(m_autoSaveWidget);
    mainLayout->addWidget(m_canvasColorWidget);
    mainLayout->addStretch(0);

    m_autoSaveWidget->setObjectName(u"m_autoSaveWidget"_s);
    m_canvasColorWidget->setObjectName(u"m_canvasColorWidget"_s);
}

ConfigureGeneralWidget::~ConfigureGeneralWidget() = default;

void ConfigureGeneralWidget::save()
{
    m_autoSaveWidget->save();
    m_canvasColorWidget->save();
    DrawyGlobalConfig::self()->save();
}

void ConfigureGeneralWidget::load()
{
    m_autoSaveWidget->load();
    m_canvasColorWidget->load();
}

void ConfigureGeneralWidget::restoreToDefaults()
{
    const bool bUseDefaults = DrawyGlobalConfig::self()->useDefaults(true);
    m_autoSaveWidget->load();
    m_canvasColorWidget->load();
    DrawyGlobalConfig::self()->useDefaults(bUseDefaults);
}

#include "moc_configuregeneralwidget.cpp"
