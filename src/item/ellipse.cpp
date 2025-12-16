/*
 * Drawy - A simple brainstorming tool with an infinite canvas
 * Copyright (C) 2025 - Prayag Jain <prayagjain2@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ellipse.hpp"

EllipseItem::EllipseItem() {
}

void EllipseItem::m_draw(QPainter &painter, const QPointF &offset) const {
    painter.drawEllipse(QRectF(start() - offset, end() - offset));
}

bool EllipseItem::onEllipse(QLineF line) const {
    int sw{boundingBoxPadding() + property(Property::StrokeWidth).value<int>()};
    double bX{m_boundingBox.x() + sw}, bY{m_boundingBox.y() + sw};
    double bW{m_boundingBox.width() - 2 * sw}, bH{m_boundingBox.height() - 2 * sw};

    double h{bX + bW / 2}, k{bY + bH / 2};
    double a{bW / 2}, b{bH / 2};
    double x1{line.x1()}, y1{line.y1()};
    double x2{line.x2()}, y2{line.y2()};

    double p{x2 - x1}, q{y2 - y1};

    double as{a * a};
    double bs{b * b};
    double ps{p * p};
    double qs{q * q};

    double firstTerm{ps * bs + qs * as};
    double secondTerm{2 * (x1 * p * bs - p * h * bs + y1 * q * as - q * k * as)};
    double thirdTerm{x1 * x1 * bs + bs * h * h - 2 * x1 * h * bs +
             y1 * y1 * as + as * k * k - 2 * y1 * k * as - as * bs};

    double discriminant{secondTerm * secondTerm - 4 * firstTerm * thirdTerm};
    if (discriminant < 0)
        return false;

    double t1{(-secondTerm + sqrt(discriminant)) / (2.0 * firstTerm)};
    double t2{(-secondTerm - sqrt(discriminant)) / (2.0 * firstTerm)};

    return (t1 >= 0.0 && t1 <= 1.0) || (t2 >= 0.0 && t2 <= 1.0);
}

bool EllipseItem::intersects(const QRectF &rect) {
    if (!boundingBox().intersects(rect))
        return false;

    QPointF a{rect.topLeft()};
    QPointF b{rect.topRight()};
    QPointF c{rect.bottomRight()};
    QPointF d{rect.bottomLeft()};
    return onEllipse({a, b}) || onEllipse({b, c}) || onEllipse({c, d}) || onEllipse({d, a});
};

bool EllipseItem::intersects(const QLineF &line) {
    return onEllipse(line);
}

Item::Type EllipseItem::type() const {
    return Item::Ellipse;
}
