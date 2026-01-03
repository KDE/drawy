// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "line.hpp"

#include "common/utils/math.hpp"

LineItem::LineItem()
{
}

void LineItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    painter.drawLine(start() - offset, end() - offset);
}

bool LineItem::intersects(const QRectF &rect)
{
    return Common::Utils::Math::intersects(rect, QLineF{start(), end()});
};

bool LineItem::intersects(const QLineF &line)
{
    return Common::Utils::Math::intersects(QLineF{start(), end()}, line);
}

Item::Type LineItem::type() const
{
    return Item::Line;
}
