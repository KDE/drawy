// SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "canvascolorwidget.hpp"
#include "drawyglobalconfig.h"

#include <KColorButton>
#include <KLocalizedString>
#include <QFormLayout>

CanvasColorWidget::CanvasColorWidget(QWidget *parent)
    : ConfigGroupInterface{parent, i18n("Canvas")}
    , m_lightThemeColor{new KColorButton{DrawyGlobalConfig::backgroundColorLight(), this}}
    , m_darkThemeColor{new KColorButton{DrawyGlobalConfig::backgroundColorDark(), this}}
{
    const auto widget{new QWidget{this}};
    const auto layout{new QFormLayout{widget}};
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addRow(i18n("Light background color"), m_lightThemeColor);
    layout->addRow(i18n("Dark background color"), m_darkThemeColor);

    m_layout->addWidget(widget);
}

void CanvasColorWidget::save()
{
    DrawyGlobalConfig::setBackgroundColorLight(m_lightThemeColor->color());
    DrawyGlobalConfig::setBackgroundColorDark(m_darkThemeColor->color());
    DrawyGlobalConfig::self()->save();
}

void CanvasColorWidget::load()
{
    m_lightThemeColor->setColor(DrawyGlobalConfig::backgroundColorLight());
    m_darkThemeColor->setColor(DrawyGlobalConfig::backgroundColorDark());
}

#include "moc_canvascolorwidget.cpp"
