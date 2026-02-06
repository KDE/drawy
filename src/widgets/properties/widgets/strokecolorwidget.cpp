// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "strokecolorwidget.hpp"

#include <QColor>
#include <QPushButton>
#include <qtoolbutton.h>

#include "properties/property.hpp"
using namespace Qt::Literals::StringLiterals;
StrokeColorWidget::StrokeColorWidget(QWidget *parent)
    : ColorWidgetBase{parent}
{
    initialize();
}

QString StrokeColorWidget::name() const
{
    return tr("Stroke Color");
}

const Property StrokeColorWidget::value() const
{
    return Property{m_currentColorButton->property("color-value"), Property::Type::StrokeColor};
}

QList<QColor> StrokeColorWidget::defaultColors() const
{
    const QList<QColor> colors{QColor{255, 255, 255}, QColor{0, 0, 0}, QColor{255, 53, 71}, QColor{255, 187, 51}, QColor{51, 181, 229}};
    return colors;
}

#include "moc_strokecolorwidget.cpp"
