// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "openarrowhead.hpp"
#include "common/utils/math.hpp"
#include <QPainterPath>

OpenArrowHead::OpenArrowHead(const QPointF &start, const QPointF &end)
    : ArrowHead{start, end}
{
}

QPainterPath OpenArrowHead::getPath()
{
    QPainterPath path;

    using namespace Common::Utils::Math;
    const auto length{dist(m_start, m_end) / 2.0};
    const auto vec{perp(m_start - m_end) * length};

    const auto firstPoint{vec + m_end};
    const auto secondPoint{-vec + m_end};

    path.moveTo(m_start);
    path.lineTo(m_end);

    path.lineTo(m_start);
    path.lineTo(firstPoint);

    path.lineTo(m_start);
    path.lineTo(secondPoint);

    return path;
}

bool OpenArrowHead::isFilled() const
{
    return false;
}

ArrowHead::Type OpenArrowHead::type() const
{
    return ArrowHead::Type::Open;
}
