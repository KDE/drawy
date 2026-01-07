// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "polygon.hpp"

PolygonItem::PolygonItem()
{
    m_properties[Property::Type::StrokeWidth] = Property{1, Property::Type::StrokeWidth};
    m_properties[Property::Type::StrokeColor] = Property{QColor(Qt::black), Property::Type::StrokeColor};
    m_properties[Property::Type::Opacity] = Property{255, Property::Type::Opacity};
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
    m_updateBoundingBox();
}

const QPointF &PolygonItem::start() const
{
    return m_start;
}

const QPointF &PolygonItem::end() const
{
    return m_end;
}

void PolygonItem::m_updateBoundingBox()
{
    const double minX{std::min(m_start.x(), m_end.x())};
    const double maxX{std::max(m_start.x(), m_end.x())};
    const double minY{std::min(m_start.y(), m_end.y())};
    const double maxY{std::max(m_start.y(), m_end.y())};
    const int w{property(Property::Type::StrokeWidth).value<int>()};

    m_boundingBox = QRectF{QPointF{minX, maxY}, QPointF{maxX, minY}}.normalized();
    m_boundingBox.adjust(-w, -w, w, w);
}

void PolygonItem::draw(QPainter &painter, const QPointF &offset)
{
    QPen pen{};

    QColor color{property(Property::Type::StrokeColor).value<QColor>()};
    color.setAlpha(property(Property::Type::Opacity).value<int>());

    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(property(Property::Type::StrokeWidth).value<int>());
    pen.setColor(color);

    painter.setPen(pen);

    drawItem(painter, offset);
}

void PolygonItem::erase(QPainter &painter, const QPointF &offset) const
{
    QPen pen{};

    pen.setWidth(property(Property::Type::StrokeWidth).value<int>() * 10);
    pen.setColor(Qt::transparent);

    painter.save();
    painter.setPen(pen);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    drawItem(painter, offset);

    painter.restore();
}

void PolygonItem::translate(const QPointF &amount)
{
    m_start += amount;
    m_end += amount;

    m_updateBoundingBox();
}
