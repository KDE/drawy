// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "colorwidgetbase.hpp"
class StrokeColorWidget : public ColorWidgetBase
{
    Q_OBJECT
public:
    explicit StrokeColorWidget(QWidget *parent = nullptr);

    [[nodiscard]] QString name() const override;
    [[nodiscard]] const Property value() const override;
};
