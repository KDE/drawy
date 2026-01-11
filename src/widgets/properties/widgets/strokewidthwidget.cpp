// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "strokewidthwidget.hpp"
#include "drawyglobalconfig.h"
#include <QSpinBox>

#include "properties/property.hpp"

StrokeWidthWidget::StrokeWidthWidget(QWidget *parent)
    : PropertyWidget{parent}
{
    QSpinBox *box{new QSpinBox(parent)};
    box->setRange(1, 10);
    box->setValue(DrawyGlobalConfig::self()->strokeWidth());

    box->hide();
    m_widget = box;

    connect(box, &QSpinBox::valueChanged, this, [this]() {
        Q_EMIT changed(value());
    });
}

QString StrokeWidthWidget::name() const
{
    return tr("Thickness");
}

const Property StrokeWidthWidget::value() const
{
    return Property{static_cast<QSpinBox *>(m_widget)->value(), Property::Type::StrokeWidth};
}

#include "moc_strokewidthwidget.cpp"
