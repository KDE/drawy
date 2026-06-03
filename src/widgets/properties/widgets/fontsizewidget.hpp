// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "propertywidget.hpp"
class QSpinBox;
class FontSizeWidget : public PropertyWidget
{
    Q_OBJECT
public:
    explicit FontSizeWidget(QWidget *parent = nullptr);

    [[nodiscard]] QString name() const override;
    [[nodiscard]] Property value() const override;

    void setValue(const QVariant &value) override;

private:
    QSpinBox *const mSpinBox;
};
