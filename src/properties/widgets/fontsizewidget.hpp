// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "propertywidget.hpp"

class FontSizeWidget : public PropertyWidget
{
public:
    explicit FontSizeWidget(QWidget *parent = nullptr);

    QString name() const override;
    const Property value() const override;
};
