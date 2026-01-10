// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "rectangle.hpp"

#include "common/utils/math.hpp"
#include "serializer/rectangledeserializer.hpp"
#include "serializer/rectangleserializer.hpp"
#include <QJsonObject>

RectangleItem::RectangleItem() = default;

void RectangleItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    painter.drawRect(QRectF(start() - offset, end() - offset));
}

bool RectangleItem::intersects(const QRectF &rect)
{
    if (!boundingBox().intersects(rect))
        return false;

    const QRectF box{start(), end()};
    const QPointF p{box.topLeft()};
    const QPointF q{box.topRight()};
    const QPointF r{box.bottomRight()};
    const QPointF s{box.bottomLeft()};

    const QPointF a{rect.topLeft()};
    const QPointF b{rect.topRight()};
    const QPointF c{rect.bottomRight()};
    const QPointF d{rect.bottomLeft()};

    return (Common::Utils::Math::intersects(QLineF{p, q}, QLineF{a, b}) || Common::Utils::Math::intersects(QLineF{p, q}, QLineF{b, c})
            || Common::Utils::Math::intersects(QLineF{p, q}, QLineF{c, d}) || Common::Utils::Math::intersects(QLineF{p, q}, QLineF{d, a})
            || Common::Utils::Math::intersects(QLineF{q, r}, QLineF{a, b}) || Common::Utils::Math::intersects(QLineF{q, r}, QLineF{b, c})
            || Common::Utils::Math::intersects(QLineF{q, r}, QLineF{c, d}) || Common::Utils::Math::intersects(QLineF{q, r}, QLineF{d, a})
            || Common::Utils::Math::intersects(QLineF{r, s}, QLineF{a, b}) || Common::Utils::Math::intersects(QLineF{r, s}, QLineF{b, c})
            || Common::Utils::Math::intersects(QLineF{r, s}, QLineF{c, d}) || Common::Utils::Math::intersects(QLineF{r, s}, QLineF{d, a})
            || Common::Utils::Math::intersects(QLineF{p, s}, QLineF{a, b}) || Common::Utils::Math::intersects(QLineF{p, s}, QLineF{b, c})
            || Common::Utils::Math::intersects(QLineF{p, s}, QLineF{c, d}) || Common::Utils::Math::intersects(QLineF{p, s}, QLineF{d, a}));
}

bool RectangleItem::intersects(const QLineF &line)
{
    const QRectF box{start(), end()};
    return Common::Utils::Math::intersects(box, line);
}

Item::Type RectangleItem::type() const
{
    return Item::Type::Rectangle;
}

QJsonObject RectangleItem::serialize() const
{
    const RectangleSerializer serialize(this);
    return serialize.serialize();
}

void RectangleItem::deserialize(const QJsonObject &obj)
{
    RectangleDeserializer deserializer(this);
    deserializer.deserialize(obj);
}

bool RectangleItem::operator==(const RectangleItem &other) const
{
    return start() == other.start() && PolygonItem::operator==(other);
}
