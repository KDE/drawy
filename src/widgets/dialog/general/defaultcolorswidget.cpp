// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defaultcolorswidget.hpp"
#include "drawyglobalconfig.h"
#include <KColorButton>
#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>
#include <QStyle>

DefaultColorsWidget::DefaultColorsWidget(QWidget *parent)
    : ConfigGroupInterface{parent, i18n("Color Palette")}
    , m_lightForegroundColor{new KColorButton{DrawyGlobalConfig::foregroundColorLight(), this}}
    , m_darkForegroundColor{new KColorButton{DrawyGlobalConfig::foregroundColorDark(), this}}
    , m_lightFirstColor{new KColorButton{DrawyGlobalConfig::firstColorLight(), this}}
    , m_lightSecondColor{new KColorButton{DrawyGlobalConfig::secondColorLight(), this}}
    , m_lightThirdColor{new KColorButton{DrawyGlobalConfig::thirdColorLight(), this}}
    , m_lightFourthColor{new KColorButton{DrawyGlobalConfig::fourthColorLight(), this}}
    , m_darkFirstColor{new KColorButton{DrawyGlobalConfig::firstColorDark(), this}}
    , m_darkSecondColor{new KColorButton{DrawyGlobalConfig::secondColorDark(), this}}
    , m_darkThirdColor{new KColorButton{DrawyGlobalConfig::thirdColorDark(), this}}
    , m_darkFourthColor{new KColorButton{DrawyGlobalConfig::fourthColorDark(), this}}
{
    const auto widget = new QWidget{this};
    const auto layout = new QFormLayout{widget};
    layout->setContentsMargins(0, 0, 0, 0);

    auto makeRow = [](std::initializer_list<QWidget *> widgets) -> QWidget * {
        auto container = new QWidget;
        auto h = new QHBoxLayout{container};
        h->setContentsMargins(0, 0, 0, 0);

        for (auto w : widgets) {
            h->addWidget(w);
        }

        h->addStretch();
        return container;
    };

    // Light row
    layout->addRow(i18n("Light Mode"), makeRow({m_lightForegroundColor, m_lightFirstColor, m_lightSecondColor, m_lightThirdColor, m_lightFourthColor}));

    // Dark row
    layout->addRow(i18n("Dark Mode"), makeRow({m_darkForegroundColor, m_darkFirstColor, m_darkSecondColor, m_darkThirdColor, m_darkFourthColor}));

    m_layout->addWidget(widget);
}

void DefaultColorsWidget::save()
{
    DrawyGlobalConfig::setForegroundColorLight(m_lightForegroundColor->color());
    DrawyGlobalConfig::setForegroundColorDark(m_darkForegroundColor->color());

    DrawyGlobalConfig::setFirstColorLight(m_lightFirstColor->color());
    DrawyGlobalConfig::setSecondColorLight(m_lightSecondColor->color());
    DrawyGlobalConfig::setThirdColorLight(m_lightThirdColor->color());
    DrawyGlobalConfig::setFourthColorLight(m_lightFourthColor->color());

    DrawyGlobalConfig::setFirstColorDark(m_darkFirstColor->color());
    DrawyGlobalConfig::setSecondColorDark(m_darkSecondColor->color());
    DrawyGlobalConfig::setThirdColorDark(m_darkThirdColor->color());
    DrawyGlobalConfig::setFourthColorDark(m_darkFourthColor->color());

    DrawyGlobalConfig::self()->save();
}

void DefaultColorsWidget::load()
{
    m_lightForegroundColor->setColor(DrawyGlobalConfig::foregroundColorLight());
    m_darkForegroundColor->setColor(DrawyGlobalConfig::foregroundColorDark());

    m_lightFirstColor->setColor(DrawyGlobalConfig::firstColorLight());
    m_lightSecondColor->setColor(DrawyGlobalConfig::secondColorLight());
    m_lightThirdColor->setColor(DrawyGlobalConfig::thirdColorLight());
    m_lightFourthColor->setColor(DrawyGlobalConfig::fourthColorLight());

    m_darkFirstColor->setColor(DrawyGlobalConfig::firstColorDark());
    m_darkSecondColor->setColor(DrawyGlobalConfig::secondColorDark());
    m_darkThirdColor->setColor(DrawyGlobalConfig::thirdColorDark());
    m_darkFourthColor->setColor(DrawyGlobalConfig::fourthColorDark());
}

#include "moc_defaultcolorswidget.cpp"
