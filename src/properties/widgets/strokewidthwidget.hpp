// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "propertywidget.hpp"

class StrokeWidthWidget : public PropertyWidget {
public:
    explicit StrokeWidthWidget(QWidget *parent = nullptr);

    QString name() const override;
    const Property value() const override;
};
