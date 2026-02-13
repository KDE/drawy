// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "polygon.hpp"
#include "item/itemutils.hpp"
#include "serializer/polygondeserializer.hpp"
#include "serializer/polygonserializer.hpp"
#include <QJsonObject>
#include <qnamespace.h>
using namespace Qt::Literals::StringLiterals;
PolygonItem::PolygonItem()
{
    m_properties[Property::Type::StrokeWidth] = Property{1, Property::Type::StrokeWidth};
    m_properties[Property::Type::StrokeColor] = Property{QColor(Qt::black), Property::Type::StrokeColor};
    m_properties[Property::Type::Opacity] = Property{255, Property::Type::Opacity};
    m_properties[Property::Type::StrokeStyle] = Property{ItemUtils::convertItemStrokeTypeEnumToString(Item::StrokeType::Solid), Property::Type::StrokeStyle};
}

void PolygonItem::setStart(QPointF start)
{
    m_start = start;
    m_end = start;
    m_boundingBox = QRectF{start, start};
}

void PolygonItem::setEnd(QPointF end)
{
    m_end = end;
    updateBoundingBox();
}

void PolygonItem::setEndWithShift(QPointF end)
{
    const QPointF size{end - m_start};
    const qreal maxDimension{std::max(std::abs(size.x()), std::abs(size.y()))};

    m_end = m_start + QPointF{std::copysign(maxDimension, size.x()), std::copysign(maxDimension, size.y())};
    updateBoundingBox();
}

const QPointF &PolygonItem::start() const
{
    return m_start;
}

const QPointF &PolygonItem::end() const
{
    return m_end;
}

QJsonObject PolygonItem::serialize(int zorder) const
{
    const PolygonSerializer serialize(this);
    return serialize.serialize(zorder);
}

void PolygonItem::updateBoundingBox()
{
    const double minX{std::min(m_start.x(), m_end.x())};
    const double maxX{std::max(m_start.x(), m_end.x())};
    const double minY{std::min(m_start.y(), m_end.y())};
    const double maxY{std::max(m_start.y(), m_end.y())};
    const int w{property(Property::Type::StrokeWidth).value<int>()};

    m_boundingBox = QRectF{QPointF{minX, maxY}, QPointF{maxX, minY}}.normalized();
    m_boundingBox.adjust(-w, -w, w, w);
}

void PolygonItem::prepareBackground(QPainter &painter) const
{
    QColor backgroundColor{property(Property::Type::BackgroundColor).value<QColor>()};
    if (backgroundColor != Qt::transparent) {
        backgroundColor.setAlpha(property(Property::Type::Opacity).value<int>());
        const Qt::BrushStyle brushStyle{ItemUtils::convertItemBackgroundTypeStringToBrushStyle(property(Property::Type::BackgroundStyle).value<QString>())};
        painter.setBrush(QBrush(backgroundColor, brushStyle));
    }
}

void PolygonItem::draw(QPainter &painter, const QPointF &offset)
{
    QPen pen;

    QColor color{property(Property::Type::StrokeColor).value<QColor>()};
    color.setAlpha(property(Property::Type::Opacity).value<int>());
    pen.setStyle(ItemUtils::convertItemStrokeTypeStringToPenStyle(property(Property::Type::StrokeStyle).value<QString>()));

    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(property(Property::Type::StrokeWidth).value<int>());
    pen.setColor(color);

    painter.setPen(pen);

    drawItem(painter, offset);
}

void PolygonItem::deserialize(const QJsonObject &obj)
{
    PolygonDeserializer deserializer(this);
    deserializer.deserialize(obj);
}

bool PolygonItem::operator==(const PolygonItem &other) const
{
    return m_start == other.start() && m_end == other.end() && Item::operator==(other);
}

void PolygonItem::normalize()
{
    m_end -= m_start;

    m_boundingBox.translate(-m_start);
    m_transform.translate(m_start.x(), m_start.y());
    m_start.setX(0);
    m_start.setY(0);
}

QDebug operator<<(QDebug d, const PolygonItem &t)
{
    d.space() << "start:" << t.start();
    d.space() << "end:" << t.end();
    d.space() << "Item: " << static_cast<const Item &>(t);
    return d;
}
