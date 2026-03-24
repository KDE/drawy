// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "diamondarrowhead.hpp"
#include "common/utils/math.hpp"
#include <QPainterPath>

DiamondArrowHead::DiamondArrowHead(const QPointF &start, const QPointF &end)
    : ArrowHead{start, end}
{
}

QPainterPath DiamondArrowHead::getPath()
{
    QPainterPath path{};

    using namespace Common::Utils::Math;
    const auto midPoint{(m_start + m_end) / 2.0};
    const auto length{dist(m_start, m_end) / 3.0};
    const auto vec{perp(m_start - m_end) * length};

    const auto firstPoint{vec + midPoint};
    const auto secondPoint{-vec + midPoint};

    path.moveTo(m_end);
    path.lineTo(firstPoint);
    path.lineTo(m_start);
    path.lineTo(secondPoint);
    path.closeSubpath();

    return path;
}

bool DiamondArrowHead::isFilled() const
{
    return false;
}

ArrowHead::Type DiamondArrowHead::type() const
{
    return ArrowHead::Type::Diamond;
}
