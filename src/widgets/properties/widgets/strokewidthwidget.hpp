// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "propertywidget.hpp"

class StrokeWidthWidget : public PropertyWidget
{
    Q_OBJECT
public:
    explicit StrokeWidthWidget(QWidget *parent = nullptr);

    [[nodiscard]] QString name() const override;
    [[nodiscard]] const Property value() const override;
};
