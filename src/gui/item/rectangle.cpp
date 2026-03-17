// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "rectangle.hpp"

#include "item/itemutils.hpp"
#include "serializer/rectangledeserializer.hpp"
#include "serializer/rectangleserializer.hpp"
#include <QJsonObject>
#include <QPainterPath>

RectangleItem::RectangleItem()
{
}

void RectangleItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    prepareBackground(painter);
    painter.drawRect(QRectF(start() - offset, end() - offset));
}

bool RectangleItem::intersects(const QRectF &rect)
{
    if (!boundingBox().intersects(rect)) {
        return false;
    }

    QPainterPath path{};
    path.addRect(QRectF{start(), end()});

    path = transformObj().map(path);

    if (isFilled()) {
        return path.intersects(rect);
    }

    return path.intersects(rect) && !path.contains(rect);
}

Item::FormType RectangleItem::formType() const
{
    return Item::FormType::Rectangle;
}

QJsonObject RectangleItem::serialize(int zorder) const
{
    const RectangleSerializer serialize(this);
    return serialize.serialize(zorder);
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
