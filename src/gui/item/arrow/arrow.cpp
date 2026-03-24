// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "arrow.hpp"

#include "arrowutils.hpp"
#include "common/utils/math.hpp"
#include "serializer/arrowdeserializer.hpp"
#include "serializer/arrowserializer.hpp"
#include <QJsonObject>
#include <QPainterPath>

ArrowItem::ArrowItem()
{
    m_properties[Property::Type::StartArrowType] = Property{ArrowUtils::toString(ArrowHead::Type::None), Property::Type::StartArrowType};
    m_properties[Property::Type::EndArrowType] = Property{ArrowUtils::toString(ArrowHead::Type::Open), Property::Type::EndArrowType};
}

void ArrowItem::setStart(QPointF start)
{
    LineItem::setStart(start);
    updatePath();
}

void ArrowItem::setEnd(QPointF end)
{
    LineItem::setEnd(end);
    updatePath();
}

void ArrowItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    painter.save();
    painter.translate(-offset);

    // to hide the intersection points
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    // we can't draw the path directly since we need to take care of the fact
    // that an arrow could have filled and unfilled heads
    painter.drawLine(getLineStart(), getLineEnd());

    if (m_startArrowHead->isFilled()) {
        painter.setBrush(painter.pen().color());
    } else {
        painter.setBrush(Qt::NoBrush);
    }

    painter.drawPath(m_startArrowHead->getPath());

    if (m_endArrowHead->isFilled()) {
        painter.setBrush(painter.pen().color());
    } else {
        painter.setBrush(Qt::NoBrush);
    }

    painter.drawPath(m_endArrowHead->getPath());

    painter.restore();
}

void ArrowItem::commitTransformation()
{
    LineItem::commitTransformation();
    updatePath();
}

Item::FormType ArrowItem::formType() const
{
    return Item::FormType::Arrow;
}

QPointF ArrowItem::getLineStart() const
{
    const auto arrowLength{std::min(arrowHeadMaxWidth, Common::Utils::Math::dist(start(), end()) * 0.5)};
    return Common::Utils::Math::unitVector(end() - start()) * arrowLength + start();
}

QPointF ArrowItem::getLineEnd() const
{
    const qreal arrowLength{std::min(arrowHeadMaxWidth, Common::Utils::Math::dist(start(), end()) * 0.5)};
    return Common::Utils::Math::unitVector(start() - end()) * arrowLength + end();
}

QJsonObject ArrowItem::serialize(int zorder) const
{
    const ArrowSerializer serializer(this);
    return serializer.serialize(zorder);
}

void ArrowItem::deserialize(const QJsonObject &obj)
{
    ArrowDeserializer deserializer(this);
    deserializer.deserialize(obj);
}

void ArrowItem::updatePath()
{
    m_path.clear();

    ArrowHead::Type startType{ArrowUtils::fromString(property(Property::Type::StartArrowType).value<QString>())};
    ArrowHead::Type endType{ArrowUtils::fromString(property(Property::Type::EndArrowType).value<QString>())};

    m_startArrowHead = ArrowHead::create(startType, start(), getLineStart());
    m_endArrowHead = ArrowHead::create(endType, end(), getLineEnd());

    m_path.addPath(m_startArrowHead->getPath());
    m_path.addPath(m_endArrowHead->getPath());

    m_path.moveTo(getLineStart());
    m_path.lineTo(getLineEnd());

    // to prevent artifacts
    constexpr static int padding{10};
    m_boundingBox = m_path.boundingRect().toAlignedRect().adjusted(-padding, -padding, padding, padding);
}

bool ArrowItem::intersects(const QRectF &rect)
{
    return transformObj().map(m_path).intersects(rect);
}

void ArrowItem::updateAfterProperty()
{
    updatePath();
    Item::updateAfterProperty();
}

bool ArrowItem::operator==(const ArrowItem &other) const
{
    return LineItem::operator==(other);
}

QDebug operator<<(QDebug d, const ArrowItem &t)
{
    d.space() << "LineItem: " << static_cast<const LineItem &>(t);
    return d;
}
