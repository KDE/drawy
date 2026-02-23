// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "line.hpp"

#include "common/utils/math.hpp"
#include "serializer/linedeserializer.hpp"
#include "serializer/lineserializer.hpp"
#include <QJsonObject>
#include <QPainterPath>

LineItem::LineItem() = default;

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

QList<TransformHandlerUtils::Type> LineItem::transformHandlers() const
{
    using enum TransformHandlerUtils::Type;
    return {LineResizeTransformHandler, LineMoveTransformHandler};
}
