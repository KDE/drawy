// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "nonearrowhead.hpp"
#include <QPainterPath>

NoneArrowHead::NoneArrowHead(const QPointF &start, const QPointF &end)
    : ArrowHead{start, end}
{
}

QPainterPath NoneArrowHead::getPath()
{
    QPainterPath path{};

    path.moveTo(m_start);
    path.lineTo(m_end);

    return path;
}

bool NoneArrowHead::isFilled() const
{
    return false;
}

ArrowHead::Type NoneArrowHead::type() const
{
    return ArrowHead::Type::None;
}
