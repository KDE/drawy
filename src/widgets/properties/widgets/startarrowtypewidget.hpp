// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "arrowtypewidget.hpp"

class StartArrowTypeWidget : public ArrowTypeWidget
{
    Q_OBJECT
public:
    explicit StartArrowTypeWidget(QWidget *parent = nullptr);

    [[nodiscard]] QString name() const override;
    [[nodiscard]] Property value() const override;
};
