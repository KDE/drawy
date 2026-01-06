// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ellipse.hpp"

EllipseItem::EllipseItem() = default;

void EllipseItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    painter.drawEllipse(QRectF(start() - offset, end() - offset));
}

bool EllipseItem::onEllipse(QLineF line) const
{
    int sw{boundingBoxPadding() + property(Property::Type::StrokeWidth).value<int>()};
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
    double thirdTerm{x1 * x1 * bs + bs * h * h - 2 * x1 * h * bs + y1 * y1 * as + as * k * k - 2 * y1 * k * as - as * bs};

    double discriminant{secondTerm * secondTerm - 4 * firstTerm * thirdTerm};
    if (discriminant < 0)
        return false;

    double t1{(-secondTerm + sqrt(discriminant)) / (2.0 * firstTerm)};
    double t2{(-secondTerm - sqrt(discriminant)) / (2.0 * firstTerm)};

    return (t1 >= 0.0 && t1 <= 1.0) || (t2 >= 0.0 && t2 <= 1.0);
}

bool EllipseItem::intersects(const QRectF &rect)
{
    if (!boundingBox().intersects(rect))
        return false;

    QPointF a{rect.topLeft()};
    QPointF b{rect.topRight()};
    QPointF c{rect.bottomRight()};
    QPointF d{rect.bottomLeft()};
    return onEllipse({a, b}) || onEllipse({b, c}) || onEllipse({c, d}) || onEllipse({d, a});
};

bool EllipseItem::intersects(const QLineF &line)
{
    return onEllipse(line);
}

Item::Type EllipseItem::type() const
{
    return Item::Type::Ellipse;
}
