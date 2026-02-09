// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "strokecolorwidget.hpp"

#include <QColor>
#include <QToolButton>

#include "properties/property.hpp"
#include "properties/propertyutils.hpp"
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
    return PropertyUtils::strokeDefaultColors();
}

#include "moc_strokecolorwidget.cpp"
