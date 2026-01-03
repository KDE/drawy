// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "propertywidget.hpp"
class QButtonGroup;

class StrokeColorWidget : public PropertyWidget
{
public:
    explicit StrokeColorWidget(QWidget *parent = nullptr);

    QString name() const override;
    const Property value() const override;

private:
    QButtonGroup *m_group{};
};
