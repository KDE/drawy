// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "line.hpp"

#include "common/utils/math.hpp"
#include "item/itemutils.hpp"
#include "serializer/linedeserializer.hpp"
#include "serializer/lineserializer.hpp"
#include <QJsonObject>
#include <QPainterPath>

LineItem::LineItem()
{
    m_properties[Property::Type::StrokeWidth] = Property{1, Property::Type::StrokeWidth};
    m_properties[Property::Type::StrokeColor] = Property{QColor(Qt::black), Property::Type::StrokeColor};
    m_properties[Property::Type::Opacity] = Property{255, Property::Type::Opacity};
    m_properties[Property::Type::StrokeStyle] = Property{ItemUtils::convertItemStrokeTypeEnumToString(Item::StrokeType::Solid), Property::Type::StrokeStyle};
}

void LineItem::draw(QPainter &painter, const QPointF &offset)
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

void LineItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    painter.drawLine(start() - offset, end() - offset);
}

// Copied from Krita, with modifications
static QPointF straightLine(QPointF lineVector)
{
    qreal lineAngle = std::atan2(lineVector.y(), lineVector.x());

    if (lineAngle < 0) {
        lineAngle += 2 * M_PI;
    }

    const qreal ANGLE_BETWEEN_CONSTRAINED_LINES = (2 * M_PI) / 24;

    const quint32 constrainedLineIndex = static_cast<quint32>((lineAngle / ANGLE_BETWEEN_CONSTRAINED_LINES) + 0.5);
    const qreal constrainedLineAngle = constrainedLineIndex * ANGLE_BETWEEN_CONSTRAINED_LINES;

    const qreal lineLength = std::sqrt((lineVector.x() * lineVector.x()) + (lineVector.y() * lineVector.y()));

    const QPointF constrainedLineVector(lineLength * std::cos(constrainedLineAngle), lineLength * std::sin(constrainedLineAngle));

    return constrainedLineVector;
}

void LineItem::initPoints(QPointF start)
{
    m_start = start;
    m_end = start;
    m_boundingBox = QRectF{start, start};
}

void LineItem::setStart(QPointF start)
{
    m_start = start;
    updateBoundingBox();
}

void LineItem::setEnd(QPointF end)
{
    m_end = end;
    updateBoundingBox();
}

const QPointF &LineItem::start() const
{
    return m_start;
}

const QPointF &LineItem::end() const
{
    return m_end;
}

void LineItem::updateBoundingBox()
{
    const double minX{std::min(m_start.x(), m_end.x())};
    const double maxX{std::max(m_start.x(), m_end.x())};
    const double minY{std::min(m_start.y(), m_end.y())};
    const double maxY{std::max(m_start.y(), m_end.y())};
    const int w{property(Property::Type::StrokeWidth).value<int>()};

    m_boundingBox = QRectF{QPointF{minX, maxY}, QPointF{maxX, minY}}.normalized();
    m_boundingBox.adjust(-w, -w, w, w);
}

void LineItem::commitTransformation()
{
    const auto [scaleX, scaleY]{Common::Utils::Math::extractScale(m_transform)};
    const QTransform filtered{scaleX, 0, 0, scaleY, 0, 0};

    m_start = filtered.map(m_start);
    m_end = filtered.map(m_end);

    setDirty(true);
    updateBoundingBox();
}

void LineItem::setEndWithShift(QPointF end)
{
    setEnd(start() + straightLine(end - start()));
}

bool LineItem::intersects(const QRectF &rect)
{
    QPainterPath path{};
    path.moveTo(start());
    path.lineTo(end());

    path = m_transform.map(path);

    return path.intersects(rect);
}

Item::FormType LineItem::formType() const
{
    return Item::FormType::Line;
}

QJsonObject LineItem::serialize(int zorder) const
{
    const LineSerializer serialize(this);
    return serialize.serialize(zorder);
}

void LineItem::deserialize(const QJsonObject &obj)
{
    LineDeserializer deserializer(this);
    deserializer.deserialize(obj);
}

bool LineItem::operator==(const LineItem &other) const
{
    return m_start == other.start() && m_end == other.end() && Item::operator==(other);
}

QList<TransformHandlerUtils::Type> LineItem::transformHandlers() const
{
    using enum TransformHandlerUtils::Type;
    return {LineResizeTransformHandler, LineMoveTransformHandler};
}
