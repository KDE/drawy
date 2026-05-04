// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "colorwidgetbase.hpp"
#include "context/uicontext.hpp"
#include "drawyglobalconfig.h"
#include <KLocalizedString>
#include <KSeparator>
#include <QButtonGroup>
#include <QColor>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;

namespace
{
QList<QColor> paletteColors()
{
    const bool isDark{UIContext::isDarkTheme()};
    const auto cfg{DrawyGlobalConfig::self()};

    return {isDark ? cfg->foregroundColorDark() : cfg->foregroundColorLight(),
            isDark ? cfg->firstColorDark() : cfg->firstColorLight(),
            isDark ? cfg->secondColorDark() : cfg->secondColorLight(),
            isDark ? cfg->thirdColorDark() : cfg->thirdColorLight(),
            isDark ? cfg->fourthColorDark() : cfg->fourthColorLight()};
}
};

ColorWidgetBase::ColorWidgetBase(QWidget *parent)
    : PropertyWidget{parent}
{
    m_widget = new QWidget{parent};
    m_group = new QButtonGroup{m_widget};
}

void ColorWidgetBase::initialize()
{
    auto layout{new QHBoxLayout{m_widget}};
    layout->setContentsMargins({});
    layout->setAlignment(Qt::AlignLeft);

    auto colorButtonHLayout = new QHBoxLayout;
    colorButtonHLayout->setContentsMargins({});
    colorButtonHLayout->setSpacing(0);
    layout->addLayout(colorButtonHLayout);

    const auto colors{paletteColors()};
    for (qsizetype pos = 0; pos < colors.size(); pos++) {
        const auto color{colors[pos]};

        auto btn{new QToolButton{m_widget}};
        btn->setCheckable(true);
        btn->setStyleSheet(u"background-color: "_s + color.name());
        btn->setProperty("color-value", color);
        colorButtonHLayout->addWidget(btn);

        m_group->addButton(btn, static_cast<int>(pos));
    }

    auto separator = new KSeparator(Qt::Vertical, m_widget);
    separator->setObjectName(u"separator"_s);
    layout->addWidget(separator);

    m_currentColorButton = new QToolButton{m_widget};
    m_currentColorButton->setObjectName(u"m_currentColorButton"_s);
    m_currentColorButton->setCheckable(false);
    m_currentColorButton->setToolTip(i18nc("@info:tooltip", "Select Custom Color"));

    connect(m_currentColorButton, &QToolButton::clicked, this, [this]() {
        const QColor col = QColorDialog::getColor(m_currentColorButton->property("color-value").value<QColor>(), m_widget);
        if (col.isValid()) {
            assignCurrentColor(col);
            Q_EMIT changed(value());
        }
    });

    layout->addWidget(m_currentColorButton);

    connect(m_group, &QButtonGroup::idClicked, this, [this]() {
        const QColor selectedColor = m_group->checkedButton()->property("color-value").value<QColor>();
        assignCurrentColor(selectedColor);
        Q_EMIT changed(value());
    });

    m_group->buttons().at(0)->setChecked(true);
    assignCurrentColor(m_group->buttons().at(0)->property("color-value").value<QColor>());
    m_widget->hide();
}

void ColorWidgetBase::updateWidget()
{
    const auto colors{paletteColors()};

    for (qsizetype pos = 0; pos < colors.size(); pos++) {
        const auto btn{m_group->button(static_cast<int>(pos))};
        const auto color{colors[pos]};

        if (m_currentColorButton->property("color-value") == btn->property("color-value")) {
            assignCurrentColor(color);
        }

        btn->setCheckable(true);
        btn->setStyleSheet(u"background-color: "_s + color.name());
        btn->setProperty("color-value", color);
    }
}

void ColorWidgetBase::assignCurrentColor(const QColor &col)
{
    m_currentColorButton->setProperty("color-value", col);
    m_currentColorButton->setStyleSheet(u"QToolButton { background-color: "_s + col.name() + u"; }"_s);
}

QString ColorWidgetBase::name() const
{
    return i18n("Color");
}

void ColorWidgetBase::setValue(const QVariant &val)
{
    assignCurrentColor(val.value<QColor>());
}

#include "moc_colorwidgetbase.cpp"
