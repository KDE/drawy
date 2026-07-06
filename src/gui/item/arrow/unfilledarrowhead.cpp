// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "unfilledarrowhead.hpp"
#include "common/utils/math.hpp"
#include <QPainterPath>

UnfilledArrowHead::UnfilledArrowHead(const QPointF &start, const QPointF &end)
    : ArrowHead{start, end}
{
}

QPainterPath UnfilledArrowHead::getPath()
{
    QPainterPath path;

    using namespace Common::Utils::Math;
    const auto length{dist(m_start, m_end) / 2.0};
    const auto vec{perp(m_start - m_end) * length};

    const auto firstPoint{vec + m_end};
    const auto secondPoint{-vec + m_end};

    path.moveTo(m_start);
    path.lineTo(firstPoint);
    path.lineTo(secondPoint);
    path.closeSubpath();

    return path;
}

bool UnfilledArrowHead::isFilled() const
{
    return false;
}

ArrowHead::Type UnfilledArrowHead::type() const
{
    return ArrowHead::Type::Unfilled;
}
