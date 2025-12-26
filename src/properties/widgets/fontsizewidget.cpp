// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fontsizewidget.hpp"

#include <QSpinBox>

#include "../property.hpp"

FontSizeWidget::FontSizeWidget(QWidget *parent) : PropertyWidget{parent} {
    // TODO: Remove magic numbers
    QSpinBox *box{new QSpinBox(parent)};
    box->setMinimum(1);
    box->setValue(24);

    box->hide();
    m_widget = box;

    QObject::connect(box, &QSpinBox::valueChanged, this, [this]() { emit changed(value()); });
}

QString FontSizeWidget::name() const {
    return "Font Size";
};

const Property FontSizeWidget::value() const {
    return Property{dynamic_cast<QSpinBox *>(m_widget)->value(), Property::FontSize};
}
