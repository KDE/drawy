// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "propertywidget.hpp"

PropertyWidget::PropertyWidget(QWidget *parent) : QObject{parent} {
}

QWidget *PropertyWidget::widget() const {
    return m_widget;
}
