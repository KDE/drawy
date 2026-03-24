// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "circlearrowhead.hpp"
#include "common/utils/math.hpp"
#include <QPainterPath>

CircleArrowHead::CircleArrowHead(const QPointF &start, const QPointF &end)
    : ArrowHead{start, end}
{
}

QPainterPath CircleArrowHead::getPath()
{
    QPainterPath path{};

    using namespace Common::Utils::Math;
    const auto radius{dist(m_start, m_end) / 2.0};
    const auto midPoint{(m_start + m_end) / 2.0};

    path.addEllipse(midPoint, radius, radius);

    return path;
}

bool CircleArrowHead::isFilled() const
{
    return false;
}

ArrowHead::Type CircleArrowHead::type() const
{
    return ArrowHead::Type::Circle;
}
