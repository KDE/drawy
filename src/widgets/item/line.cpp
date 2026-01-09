// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "line.hpp"

#include "common/utils/math.hpp"
#include "serializer/linedeserializer.hpp"
#include "serializer/lineserializer.hpp"
#include <QJsonObject>

LineItem::LineItem() = default;

void LineItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    painter.drawLine(start() - offset, end() - offset);
}

bool LineItem::intersects(const QRectF &rect)
{
    return Common::Utils::Math::intersects(rect, QLineF{start(), end()});
}

bool LineItem::intersects(const QLineF &line)
{
    return Common::Utils::Math::intersects(QLineF{start(), end()}, line);
}

Item::Type LineItem::type() const
{
    return Item::Type::Line;
}

QJsonObject LineItem::serialize() const
{
    const LineSerializer serialize(this);
    return serialize.serialize();
}

void LineItem::deserialize(const QJsonObject &obj)
{
    LineDeserializer deserializer(this);
    deserializer.deserialize(obj);
}
