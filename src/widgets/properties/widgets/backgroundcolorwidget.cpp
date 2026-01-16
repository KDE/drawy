// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "backgroundcolorwidget.hpp"

#include <QButtonGroup>
#include <QColor>
#include <QHBoxLayout>
#include <QPushButton>

#include "properties/property.hpp"
using namespace Qt::Literals::StringLiterals;
BackgroundColorWidget::BackgroundColorWidget(QWidget *parent)
    : ColorWidgetBase{parent}
{
    initialize();
}

QString BackgroundColorWidget::name() const
{
    return tr("Background Color");
}

const Property BackgroundColorWidget::value() const
{
    return Property{m_group->checkedButton()->property("color-value"), Property::Type::BackgroundColor};
}

QList<QColor> BackgroundColorWidget::defaultColors() const
{
    const QList<QColor> colors{QColor{Qt::transparent}, QColor{0, 0, 0}, QColor{255, 53, 71}, QColor{255, 187, 51}, QColor{51, 181, 229}};
    return colors;
}

#include "moc_backgroundcolorwidget.cpp"
