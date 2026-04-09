/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "configuregeneralwidget.hpp"
#include "autosavewidget.hpp"
#include "canvascolorwidget.hpp"
#include "defaultcolorswidget.hpp"
#include "drawyglobalconfig.h"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ConfigureGeneralWidget::ConfigureGeneralWidget(QWidget *parent)
    : QWidget{parent}
    , m_autoSaveWidget{new AutoSaveWidget{this}}
    , m_canvasColorWidget{new CanvasColorWidget{this}}
    , m_defaultColorsWidget{new DefaultColorsWidget{this}}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mainLayout->addWidget(m_autoSaveWidget);
    mainLayout->addWidget(m_canvasColorWidget);
    mainLayout->addWidget(m_defaultColorsWidget);
    mainLayout->addStretch(0);

    m_autoSaveWidget->setObjectName(u"m_autoSaveWidget"_s);
    m_canvasColorWidget->setObjectName(u"m_canvasColorWidget"_s);
    m_defaultColorsWidget->setObjectName(u"m_defaultColorsWidget"_s);
}

ConfigureGeneralWidget::~ConfigureGeneralWidget() = default;

void ConfigureGeneralWidget::save()
{
    m_autoSaveWidget->save();
    m_canvasColorWidget->save();
    m_defaultColorsWidget->save();
    DrawyGlobalConfig::self()->save();
}

void ConfigureGeneralWidget::load()
{
    m_autoSaveWidget->load();
    m_canvasColorWidget->load();
    m_defaultColorsWidget->load();
}

void ConfigureGeneralWidget::restoreToDefaults()
{
    const bool bUseDefaults = DrawyGlobalConfig::self()->useDefaults(true);
    m_autoSaveWidget->load();
    m_canvasColorWidget->load();
    m_defaultColorsWidget->load();
    DrawyGlobalConfig::self()->useDefaults(bUseDefaults);
}

#include "moc_configuregeneralwidget.cpp"
