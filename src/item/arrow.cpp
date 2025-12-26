// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "arrow.hpp"

#include "../common/utils/math.hpp"

ArrowItem::ArrowItem() {
}

void ArrowItem::setStart(QPointF start) {
    PolygonItem::setStart(start);
    calcArrowPoints();
}

void ArrowItem::setEnd(QPointF end) {
    PolygonItem::setEnd(end);
    calcArrowPoints();
}

void ArrowItem::calcArrowPoints() {
    double x1{start().x()}, x2{end().x()};
    double y1{start().y()}, y2{end().y()};

    qreal angle{std::atan2(y2 - y1, x2 - x1)};
    qreal arrowLength{std::sqrt(std::pow(y2 - y1, 2) + std::pow(x2 - x1, 2))};

    int maxArrowSize{static_cast<int>(m_maxArrowSize)};
    int arrowSize{std::min(maxArrowSize, static_cast<int>(arrowLength * 0.5))};

    m_arrowP1 = QPointF(x2 - arrowSize * std::cos(angle - (M_PI / 180) * 30),
                        y2 - arrowSize * std::sin(angle - (M_PI / 180) * 30));
    m_arrowP2 = QPointF(x2 - arrowSize * std::cos(angle + (M_PI / 180) * 30),
                        y2 - arrowSize * std::sin(angle + (M_PI / 180) * 30));
}

void ArrowItem::m_draw(QPainter &painter, const QPointF &offset) const {
    painter.drawLine(start() - offset, end() - offset);
    painter.drawLine(end() - offset, m_arrowP1 - offset);
    painter.drawLine(end() - offset, m_arrowP2 - offset);
}

bool ArrowItem::intersects(const QRectF &rect) {
    if (!boundingBox().intersects(rect))
        return false;

    // TODO: Use better techniques to detect collision
    QPointF p{start()}, q{end()}, r{m_arrowP1}, s{m_arrowP2};
    QPointF a{rect.x(), rect.y()};
    QPointF b{rect.x() + rect.width(), rect.y()};
    QPointF c{rect.x() + rect.width(), rect.y() + rect.height()};
    QPointF d{rect.x(), rect.y() + rect.height()};

    return (Common::Utils::Math::intersects(QLineF{p, q}, QLineF{a, b}) ||
            Common::Utils::Math::intersects(QLineF{p, q}, QLineF{b, c}) ||
            Common::Utils::Math::intersects(QLineF{p, q}, QLineF{c, d}) ||
            Common::Utils::Math::intersects(QLineF{p, q}, QLineF{d, a}) ||
            Common::Utils::Math::intersects(QLineF{q, r}, QLineF{a, b}) ||
            Common::Utils::Math::intersects(QLineF{q, r}, QLineF{b, c}) ||
            Common::Utils::Math::intersects(QLineF{q, r}, QLineF{c, d}) ||
            Common::Utils::Math::intersects(QLineF{q, r}, QLineF{d, a}) ||
            Common::Utils::Math::intersects(QLineF{q, s}, QLineF{a, b}) ||
            Common::Utils::Math::intersects(QLineF{q, s}, QLineF{b, c}) ||
            Common::Utils::Math::intersects(QLineF{q, s}, QLineF{c, d}) ||
            Common::Utils::Math::intersects(QLineF{q, s}, QLineF{d, a}));
};

bool ArrowItem::intersects(const QLineF &line) {
    return (Common::Utils::Math::intersects(QLineF{start(), end()}, line) ||
            Common::Utils::Math::intersects(QLineF{end(), m_arrowP1}, line) ||
            Common::Utils::Math::intersects(QLineF{end(), m_arrowP2}, line));
}

void ArrowItem::translate(const QPointF &amount) {
    m_arrowP1 += amount;
    m_arrowP2 += amount;

    PolygonItem::translate(amount);
};

Item::Type ArrowItem::type() const {
    return Item::Arrow;
}
