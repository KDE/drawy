// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ellipse.hpp"
#include "item/itemutils.hpp"
#include "serializer/ellipsedeserializer.hpp"
#include "serializer/ellipseserializer.hpp"
#include <QJsonObject>
#include <QPainterPath>

EllipseItem::EllipseItem()
{
    m_properties[Property::Type::BackgroundColor] = Property{QColor(Qt::transparent), Property::Type::BackgroundColor};
    m_properties[Property::Type::BackgroundStyle] =
        Property{ItemUtils::convertItemBackgroundTypeEnumToString(Item::BackgroundType::Solid), Property::Type::BackgroundStyle};
}

void EllipseItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    QColor backgroundColor{property(Property::Type::BackgroundColor).value<QColor>()};
    if (backgroundColor != Qt::transparent) {
        backgroundColor.setAlpha(property(Property::Type::Opacity).value<int>());
        const Qt::BrushStyle brushStyle{ItemUtils::convertItemBackgroundTypeStringToBrushStyle(property(Property::Type::BackgroundStyle).value<QString>())};
        painter.setBrush(QBrush(backgroundColor, brushStyle));
    }

    painter.drawEllipse(QRectF(start() - offset, end() - offset));
}

bool EllipseItem::intersects(const QRectF &rect)
{
    if (!boundingBox().intersects(rect)) {
        return false;
    }

    QPainterPath path{};
    path.addEllipse(QRectF{start(), end()});

    const bool isFilled{property(Property::Type::BackgroundColor).value<QColor>().alpha() != 0};
    if (isFilled) {
        return path.intersects(rect);
    }

    return path.intersects(rect) && !path.contains(rect);
}

Item::Type EllipseItem::type() const
{
    return Item::Type::Ellipse;
}

QJsonObject EllipseItem::serialize(int zorder) const
{
    const EllipseSerializer serializer(this);
    return serializer.serialize(zorder);
}

void EllipseItem::deserialize(const QJsonObject &obj)
{
    EllipseDeserializer deserializer(this);
    deserializer.deserialize(obj);
}
