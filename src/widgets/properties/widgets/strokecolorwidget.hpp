// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "propertywidget.hpp"
class QButtonGroup;

class StrokeColorWidget : public PropertyWidget
{
    Q_OBJECT
public:
    explicit StrokeColorWidget(QWidget *parent = nullptr);

    [[nodiscard]] QString name() const override;
    [[nodiscard]] const Property value() const override;

private:
    QButtonGroup *const m_group;
};
