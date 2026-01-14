// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "arrow.hpp"

#include "common/utils/math.hpp"
#include "serializer/arrowdeserializer.hpp"
#include "serializer/arrowserializer.hpp"
#include <QJsonObject>

// TODO add end/start arrow type.
ArrowItem::ArrowItem() = default;

void ArrowItem::setStart(QPointF start)
{
    PolygonItem::setStart(start);
    calcArrowPoints();
}

void ArrowItem::setEnd(QPointF end)
{
    PolygonItem::setEnd(end);
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
    if (m_endArrow != ArrowItem::ArrowType::None) {
        m_arrowEndP1 = QPointF(x2 - arrowSize * std::cos(angle - angleArrow), y2 - arrowSize * std::sin(angle - angleArrow));
        m_arrowEndP2 = QPointF(x2 - arrowSize * std::cos(angle + angleArrow), y2 - arrowSize * std::sin(angle + angleArrow));
    }
}

ArrowItem::ArrowType ArrowItem::endArrow() const
{
    return m_endArrow;
}

void ArrowItem::setEndArrow(const ArrowType &newEndArrow)
{
    m_endArrow = newEndArrow;
}

bool ArrowItem::operator==(const ArrowItem &other) const
{
    return m_startArrow == other.startArrow() && m_endArrow == other.endArrow() && PolygonItem::operator==(other);
}

ArrowItem::ArrowType ArrowItem::startArrow() const
{
    return m_startArrow;
}

void ArrowItem::setStartArrow(const ArrowType &newStartArrow)
{
    m_startArrow = newStartArrow;
}

void ArrowItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    painter.drawLine(start() - offset, end() - offset);
    switch (m_endArrow) {
    case ArrowType::Arrow:
        painter.drawLine(end() - offset, m_arrowEndP1 - offset);
        painter.drawLine(end() - offset, m_arrowEndP2 - offset);
        break;
    case ArrowType::Triangle:
        painter.drawLine(end() - offset, m_arrowEndP1 - offset);
        painter.drawLine(end() - offset, m_arrowEndP2 - offset);
        painter.drawLine(m_arrowEndP2 - offset, m_arrowEndP1 - offset);
        break;
    case ArrowType::None:
        break;
    }
    switch (m_startArrow) {
    case ArrowType::Arrow:
        painter.drawLine(end() - offset, m_arrowStartP1 - offset);
        painter.drawLine(end() - offset, m_arrowStartP2 - offset);
        break;
    case ArrowType::Triangle:
        painter.drawLine(end() - offset, m_arrowStartP1 - offset);
        painter.drawLine(end() - offset, m_arrowStartP2 - offset);
        painter.drawLine(m_arrowStartP2 - offset, m_arrowStartP1 - offset);
        break;
    case ArrowType::None:
        break;
    }
}

bool ArrowItem::intersects(const QRectF &rect)
{
    if (!boundingBox().intersects(rect))
        return false;

    // TODO: Use better techniques to detect collision
    const QPointF p{start()}, q{end()}, r{m_arrowEndP1}, s{m_arrowEndP2};
    const QPointF a{rect.x(), rect.y()};
    const QPointF b{rect.x() + rect.width(), rect.y()};
    const QPointF c{rect.x() + rect.width(), rect.y() + rect.height()};
    const QPointF d{rect.x(), rect.y() + rect.height()};

    return (Common::Utils::Math::intersects(QLineF{p, q}, QLineF{a, b}) || Common::Utils::Math::intersects(QLineF{p, q}, QLineF{b, c})
            || Common::Utils::Math::intersects(QLineF{p, q}, QLineF{c, d}) || Common::Utils::Math::intersects(QLineF{p, q}, QLineF{d, a})
            || Common::Utils::Math::intersects(QLineF{q, r}, QLineF{a, b}) || Common::Utils::Math::intersects(QLineF{q, r}, QLineF{b, c})
            || Common::Utils::Math::intersects(QLineF{q, r}, QLineF{c, d}) || Common::Utils::Math::intersects(QLineF{q, r}, QLineF{d, a})
            || Common::Utils::Math::intersects(QLineF{q, s}, QLineF{a, b}) || Common::Utils::Math::intersects(QLineF{q, s}, QLineF{b, c})
            || Common::Utils::Math::intersects(QLineF{q, s}, QLineF{c, d}) || Common::Utils::Math::intersects(QLineF{q, s}, QLineF{d, a}));
}

bool ArrowItem::intersects(const QLineF &line)
{
    return (Common::Utils::Math::intersects(QLineF{start(), end()}, line) || Common::Utils::Math::intersects(QLineF{end(), m_arrowEndP1}, line)
            || Common::Utils::Math::intersects(QLineF{end(), m_arrowEndP2}, line));
}

void ArrowItem::translate(const QPointF &amount)
{
    m_arrowEndP1 += amount;
    m_arrowEndP2 += amount;

    PolygonItem::translate(amount);
}

Item::Type ArrowItem::type() const
{
    return Item::Type::Arrow;
}

QJsonObject ArrowItem::serialize() const
{
    const ArrowSerializer serializer(this);
    return serializer.serialize();
}

void ArrowItem::deserialize(const QJsonObject &obj)
{
    ArrowDeserializer deserializer(this);
    deserializer.deserialize(obj);
}

QDebug operator<<(QDebug d, const ArrowItem &t)
{
    d.space() << "startArrow:" << t.startArrow();
    d.space() << "endArrow:" << t.endArrow();
    d.space() << "PolygonItem: " << static_cast<const PolygonItem &>(t);
    return d;
}
