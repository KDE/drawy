// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "arrowtypewidget.hpp"
class QButtonGroup;

class EndArrowTypeWidget : public ArrowTypeWidget
{
    Q_OBJECT
public:
    explicit EndArrowTypeWidget(QWidget *parent = nullptr);

    [[nodiscard]] QString name() const override;
    [[nodiscard]] const Property value() const override;
};
