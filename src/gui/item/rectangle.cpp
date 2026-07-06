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
    m_properties[Property::Type::CornerRectangleType] =
        Property{ItemUtils::convertItemCornerRectangleTypeEnumToString(Item::CornerRectangleType::Pointed), Property::Type::CornerRectangleType};
}

void RectangleItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    prepareBackground(painter);
    if (ItemUtils::convertItemCornerRectangleTypeStringToEnum(property(Property::Type::CornerRectangleType).value<QString>())
        == Item::CornerRectangleType::Rounded) {
        painter.drawRoundedRect(QRectF(start() - offset, end() - offset), 45, 45);
    } else {
        painter.drawRect(QRectF(start() - offset, end() - offset));
    }
}

bool RectangleItem::intersects(const QRectF &rect)
{
    if (!boundingBox().intersects(rect)) {
        return false;
    }

    QPainterPath path;
    if (ItemUtils::convertItemCornerRectangleTypeStringToEnum(property(Property::Type::CornerRectangleType).value<QString>())
        == Item::CornerRectangleType::Rounded) {
        path.addRoundedRect(QRectF{start(), end()}, 45, 45);
    } else {
        path.addRect(QRectF{start(), end()});
    }

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
