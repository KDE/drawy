// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "erasersizewidget.hpp"
#include "drawyglobalconfig.h"

#include <QSpinBox>

#include "properties/property.hpp"

EraserSizeWidget::EraserSizeWidget(QWidget *parent)
    : PropertyWidget{parent}
{
    QSpinBox *box{new QSpinBox(parent)};
    box->setRange(1, 100);
    box->setValue(DrawyGlobalConfig::self()->eraserSize());

    box->hide();
    m_widget = box;

    connect(box, &QSpinBox::valueChanged, this, [this]() {
        Q_EMIT changed(value());
    });
}

QString EraserSizeWidget::name() const
{
    return tr("Eraser Size");
}

const Property EraserSizeWidget::value() const
{
    return Property{static_cast<QSpinBox *>(m_widget)->value(), Property::Type::EraserSize};
}

#include "moc_erasersizewidget.cpp"
