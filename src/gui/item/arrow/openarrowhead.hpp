// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "arrowhead.hpp"
#include <QPainter>

class OpenArrowHead : public ArrowHead
{
public:
    OpenArrowHead(const QPointF &start, const QPointF &end);

    [[nodiscard]] QPainterPath getPath() override;
    [[nodiscard]] bool isFilled() const override;
    [[nodiscard]] ArrowHead::Type type() const override;
};
