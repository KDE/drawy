// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "arrow.hpp"

#include "serializer/arrowdeserializer.hpp"
#include "serializer/arrowserializer.hpp"
#include <QJsonObject>
#include <QPainterPath>

// TODO add end/start arrow type.
ArrowItem::ArrowItem() = default;

void ArrowItem::setStart(QPointF start)
{
    LineItem::setStart(start);
    calcArrowPoints();
}

void ArrowItem::setEnd(QPointF end)
{
    LineItem::setEnd(end);
    calcArrowPoints();
}

void ArrowItem::calcArrowPoints()
{
    const double x1{start().x()};
    const double x2{end().x()};
    const double y1{start().y()};
    const double y2{end().y()};

    const qreal angle{std::atan2(y2 - y1, x2 - x1)};
    const qreal arrowLength{std::sqrt(std::pow(y2 - y1, 2) + std::pow(x2 - x1, 2))};

    const int maxArrowSize{static_cast<int>(m_maxArrowSize)};
    const int arrowSize{std::min(maxArrowSize, static_cast<int>(arrowLength * 0.5))};

    constexpr double angleArrow = (M_PI / 180) * 30;
    if (m_endArrow != ArrowUtils::ArrowType::None) {
        m_arrowEndP1 = QPointF(x2 - arrowSize * std::cos(angle - angleArrow), y2 - arrowSize * std::sin(angle - angleArrow));
        m_arrowEndP2 = QPointF(x2 - arrowSize * std::cos(angle + angleArrow), y2 - arrowSize * std::sin(angle + angleArrow));
    }
    if (m_startArrow != ArrowUtils::ArrowType::None) {
        m_arrowStartP1 = QPointF(x1 + arrowSize * std::cos(angle - angleArrow), y1 + arrowSize * std::sin(angle - angleArrow));
        m_arrowStartP2 = QPointF(x1 + arrowSize * std::cos(angle + angleArrow), y1 + arrowSize * std::sin(angle + angleArrow));
    }
}

ArrowUtils::ArrowType ArrowItem::endArrow() const
{
    return m_endArrow;
}

void ArrowItem::setEndArrow(const ArrowUtils::ArrowType &newEndArrow)
{
    m_endArrow = newEndArrow;
}

bool ArrowItem::operator==(const ArrowItem &other) const
{
    return m_startArrow == other.startArrow() && m_endArrow == other.endArrow() && LineItem::operator==(other);
}

ArrowUtils::ArrowType ArrowItem::startArrow() const
{
    return m_startArrow;
}

void ArrowItem::setStartArrow(const ArrowUtils::ArrowType &newStartArrow)
{
    m_startArrow = newStartArrow;
}

void ArrowItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    painter.drawLine(start() - offset, end() - offset);
    switch (m_endArrow) {
    case ArrowUtils::ArrowType::Arrow: {
        const QPolygonF triangle({m_arrowEndP1, end(), m_arrowEndP2});
        QPainterPath painterPath;
        painterPath.addPolygon(triangle);
        painter.save();
        painter.translate(-offset);
        painter.drawPath(painterPath);
        painter.restore();
        break;
    }
    case ArrowUtils::ArrowType::Triangle: {
        const QPolygonF triangle({m_arrowEndP1, end(), m_arrowEndP2});
        QPainterPath painterPath;
        painterPath.addPolygon(triangle);
        painterPath.closeSubpath();
        painter.save();
        painter.translate(-offset);
        painter.drawPath(painterPath);
        painter.restore();
        break;
    }
    case ArrowUtils::ArrowType::FullTriangle: {
        const QPolygonF triangle({m_arrowEndP1, end(), m_arrowEndP2});
        QPainterPath painterPath;
        painterPath.addPolygon(triangle);
        painterPath.closeSubpath();
        painter.save();
        painter.translate(-offset);
        painter.fillPath(painterPath, painter.pen().color());
        painter.restore();
        break;
    }
    case ArrowUtils::ArrowType::Circle:
    case ArrowUtils::ArrowType::FullCircle:
        break;
    case ArrowUtils::ArrowType::Line: {
        break;
    }
    case ArrowUtils::ArrowType::None:
        // Nothing
        break;
    }

    switch (m_startArrow) {
    case ArrowUtils::ArrowType::Arrow: {
        const QPolygonF triangle({m_arrowStartP1, start(), m_arrowStartP2});
        QPainterPath painterPath;
        painterPath.addPolygon(triangle);
        painter.save();
        painter.translate(-offset);
        painter.drawPath(painterPath);
        painter.restore();
        break;
    }
    case ArrowUtils::ArrowType::Triangle: {
        const QPolygonF triangle({m_arrowStartP1, start(), m_arrowStartP2});
        QPainterPath painterPath;
        painterPath.addPolygon(triangle);
        painterPath.closeSubpath();
        painter.save();
        painter.translate(-offset);
        painter.drawPath(painterPath);
        painter.restore();
        break;
    }
    case ArrowUtils::ArrowType::FullTriangle: {
        const QPolygonF triangle({m_arrowStartP1, start(), m_arrowStartP2});
        QPainterPath painterPath;
        painterPath.addPolygon(triangle);
        painterPath.closeSubpath();
        painter.save();
        painter.translate(-offset);
        painter.fillPath(painterPath, painter.pen().color());
        painter.restore();
        break;
    }
    case ArrowUtils::ArrowType::Circle:
    case ArrowUtils::ArrowType::FullCircle:
    case ArrowUtils::ArrowType::Line:
        break;
    case ArrowUtils::ArrowType::None:
        // Nothing
        break;
    }
}

void ArrowItem::commitTransformation()
{
    LineItem::commitTransformation();
    calcArrowPoints();
}

Item::FormType ArrowItem::formType() const
{
    return Item::FormType::Arrow;
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

QDebug operator<<(QDebug d, const ArrowItem &t)
{
    d.space() << "startArrow:" << static_cast<int>(t.startArrow());
    d.space() << "endArrow:" << static_cast<int>(t.endArrow());
    d.space() << "LineItem: " << static_cast<const LineItem &>(t);
    return d;
}
