// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "backgroundcolorwidget.hpp"

#include <KLocalizedString>
#include <QColor>
#include <QToolButton>

#include "properties/property.hpp"
#include "properties/propertyutils.hpp"
using namespace Qt::Literals::StringLiterals;
BackgroundColorWidget::BackgroundColorWidget(QWidget *parent)
    : ColorWidgetBase{parent}
{
    initialize();
}

QString BackgroundColorWidget::name() const
{
    return i18n("Background Color");
}

const Property BackgroundColorWidget::value() const
{
    return Property{m_currentColorButton->property("color-value"), Property::Type::BackgroundColor};
}

QList<QColor> BackgroundColorWidget::defaultColors() const
{
    return PropertyUtils::backgroundDefaultColors();
}

#include "moc_backgroundcolorwidget.cpp"
