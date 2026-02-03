// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "line.hpp"

#include "common/utils/math.hpp"
#include "serializer/linedeserializer.hpp"
#include "serializer/lineserializer.hpp"
#include <QJsonObject>

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
    return Common::Utils::Math::intersects(rect, QLineF{start(), end()});
}

Item::Type LineItem::type() const
{
    return Item::Type::Line;
}

QJsonObject LineItem::serialize() const
{
    const LineSerializer serialize(this);
    return serialize.serialize();
}

void LineItem::deserialize(const QJsonObject &obj)
{
    LineDeserializer deserializer(this);
    deserializer.deserialize(obj);
}
