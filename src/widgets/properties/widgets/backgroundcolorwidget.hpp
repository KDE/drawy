// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "colorwidgetbase.hpp"
class BackgroundColorWidget : public ColorWidgetBase
{
    Q_OBJECT
public:
    explicit BackgroundColorWidget(QWidget *parent = nullptr);

    [[nodiscard]] QString name() const override;
    [[nodiscard]] const Property value() const override;

protected:
    [[nodiscard]] QList<QColor> defaultColors() const override;
};
