// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "diamond.hpp"

#include "common/utils/math.hpp"
#include "item/itemutils.hpp"
#include "serializer/diamonddeserializer.hpp"
#include "serializer/diamondserializer.hpp"
#include <QJsonObject>

DiamondItem::DiamondItem()
{
    m_properties[Property::Type::BackgroundColor] = Property{QColor(Qt::transparent), Property::Type::BackgroundColor};
    m_properties[Property::Type::BackgroundStyle] =
        Property{ItemUtils::convertItemBackgroundTypeEnumToString(Item::BackgroundType::Solid), Property::Type::BackgroundStyle};
}

QPainterPath DiamondItem::getPath() const
{
    const QPointF startPt = start();
    const QPointF endPt = end();
    const QPointF center = (startPt + endPt) / 2.0;

    const QPointF topPoint = QPointF(center.x(), startPt.y());
    const QPointF rightPoint = QPointF(endPt.x(), center.y());
    const QPointF bottomPoint = QPointF(center.x(), endPt.y());
    const QPointF leftPoint = QPointF(startPt.x(), center.y());

    const QPolygonF diamond({topPoint, rightPoint, bottomPoint, leftPoint});

    QPainterPath painterPath{};
    painterPath.addPolygon(diamond);
    painterPath.closeSubpath();

    return painterPath;
}

void DiamondItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    painter.save();
    painter.translate(-offset);

    QColor backgroundColor{property(Property::Type::BackgroundColor).value<QColor>()};
    if (backgroundColor != Qt::transparent) {
        backgroundColor.setAlpha(property(Property::Type::Opacity).value<int>());
        const Qt::BrushStyle brushStyle{ItemUtils::convertItemBackgroundTypeStringToBrushStyle(property(Property::Type::BackgroundStyle).value<QString>())};
        painter.setBrush(QBrush(backgroundColor, brushStyle));
    }

    painter.drawPath(getPath());

    painter.restore();
}

bool DiamondItem::intersects(const QRectF &rect)
{
    if (!boundingBox().intersects(rect)) {
        return false;
    }

    return getPath().intersects(rect);
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
