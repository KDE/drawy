// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "colorwidgetbase.hpp"

#include "properties/property.hpp"
#include <QButtonGroup>
#include <QColor>
#include <QHBoxLayout>
#include <QPushButton>
using namespace Qt::Literals::StringLiterals;
// TODO: Use a better widget
ColorWidgetBase::ColorWidgetBase(QWidget *parent)
    : PropertyWidget{parent}
{
    m_widget = new QWidget{parent};
    m_group = new QButtonGroup{m_widget};
}

void ColorWidgetBase::initialize()
{
    auto layout{new QHBoxLayout{m_widget}};
    layout->setContentsMargins(0, 0, 0, 0);

    const QList<QColor> colors = defaultColors();

    for (const QColor &color : colors) {
        QPushButton *btn{new QPushButton{m_widget}};
        btn->setCheckable(true);
        btn->setStyleSheet(u"background-color: "_s + color.name());
        btn->setProperty("color-value", color);

        layout->addWidget(btn);
        m_group->addButton(btn);
    }

    layout->setSpacing(0);
    m_group->buttons().at(0)->setChecked(true);
    m_widget->hide();

    connect(m_group, &QButtonGroup::idClicked, this, [this]() {
        Q_EMIT changed(value());
    });
}

QString ColorWidgetBase::name() const
{
    return tr("Color");
}

const Property ColorWidgetBase::value() const
{
    return Property{m_group->checkedButton()->property("color-value"), Property::Type::StrokeColor};
}

#include "moc_colorwidgetbase.cpp"
