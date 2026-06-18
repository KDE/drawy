// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "strokecolorwidget.hpp"

#include "properties/property.hpp"
#include <KLocalizedString>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;
StrokeColorWidget::StrokeColorWidget(QWidget *parent)
    : ColorWidgetBase{parent}
{
    initialize();
}

QString StrokeColorWidget::name() const
{
    return i18n("Stroke Color");
}

Property StrokeColorWidget::value() const
{
    return Property{m_currentColorButton->property("color-value"), Property::Type::StrokeColor};
}

#include "moc_strokecolorwidget.cpp"
