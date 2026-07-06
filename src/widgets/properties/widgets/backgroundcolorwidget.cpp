// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "backgroundcolorwidget.hpp"

#include "properties/property.hpp"
#include <KLocalizedString>
#include <QToolButton>
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

Property BackgroundColorWidget::value() const
{
    return Property{m_currentColorButton->property("color-value"), Property::Type::BackgroundColor};
}

#include "moc_backgroundcolorwidget.cpp"
