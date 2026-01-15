// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "diamond.hpp"

#include "common/utils/math.hpp"
#include "serializer/diamonddeserializer.hpp"
#include "serializer/diamondserializer.hpp"
#include <QJsonObject>

DiamondItem::DiamondItem()
{
    m_properties[Property::Type::BackgroundColor] = Property{QColor(Qt::transparent), Property::Type::BackgroundColor};
}

void DiamondItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    painter.drawRect(QRectF(start() - offset, end() - offset));
}

bool DiamondItem::intersects(const QRectF &rect)
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

bool DiamondItem::intersects(const QLineF &line)
{
    const QRectF box{start(), end()};
    return Common::Utils::Math::intersects(box, line);
}

Item::Type DiamondItem::type() const
{
    return Item::Type::Diamond;
}

QJsonObject DiamondItem::serialize() const
{
    const DiamondSerializer serialize(this);
    return serialize.serialize();
}

void DiamondItem::deserialize(const QJsonObject &obj)
{
    DiamondDeserializer deserializer(this);
    deserializer.deserialize(obj);
}

bool DiamondItem::operator==(const DiamondItem &other) const
{
    return start() == other.start() && PolygonItem::operator==(other);
}
