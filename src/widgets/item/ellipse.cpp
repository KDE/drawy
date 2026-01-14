// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ellipse.hpp"
#include "serializer/ellipsedeserializer.hpp"
#include "serializer/ellipseserializer.hpp"
#include <QJsonObject>

EllipseItem::EllipseItem() = default;

void EllipseItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    painter.drawEllipse(QRectF(start() - offset, end() - offset));
}

bool EllipseItem::onEllipse(QLineF line) const
{
    const int sw{boundingBoxPadding() + property(Property::Type::StrokeWidth).value<int>()};
    const double bX{m_boundingBox.x() + sw}, bY{m_boundingBox.y() + sw};
    const double bW{m_boundingBox.width() - 2 * sw}, bH{m_boundingBox.height() - 2 * sw};

    const double h{bX + bW / 2}, k{bY + bH / 2};
    const double a{bW / 2}, b{bH / 2};
    const double x1{line.x1()}, y1{line.y1()};
    const double x2{line.x2()}, y2{line.y2()};

    const double p{x2 - x1}, q{y2 - y1};

    const double as{a * a};
    const double bs{b * b};
    const double ps{p * p};
    const double qs{q * q};

    const double firstTerm{ps * bs + qs * as};
    const double secondTerm{2 * (x1 * p * bs - p * h * bs + y1 * q * as - q * k * as)};
    const double thirdTerm{x1 * x1 * bs + bs * h * h - 2 * x1 * h * bs + y1 * y1 * as + as * k * k - 2 * y1 * k * as - as * bs};

    const double discriminant{secondTerm * secondTerm - 4 * firstTerm * thirdTerm};
    if (discriminant < 0)
        return false;

    const double t1{(-secondTerm + sqrt(discriminant)) / (2.0 * firstTerm)};
    const double t2{(-secondTerm - sqrt(discriminant)) / (2.0 * firstTerm)};

    return (t1 >= 0.0 && t1 <= 1.0) || (t2 >= 0.0 && t2 <= 1.0);
}

bool EllipseItem::intersects(const QRectF &rect)
{
    if (!boundingBox().intersects(rect))
        return false;

    const QPointF a{rect.topLeft()};
    const QPointF b{rect.topRight()};
    const QPointF c{rect.bottomRight()};
    const QPointF d{rect.bottomLeft()};
    return onEllipse({a, b}) || onEllipse({b, c}) || onEllipse({c, d}) || onEllipse({d, a});
}

bool EllipseItem::intersects(const QLineF &line)
{
    return onEllipse(line);
}

Item::Type EllipseItem::type() const
{
    return Item::Type::Ellipse;
}

QJsonObject EllipseItem::serialize() const
{
    const EllipseSerializer serializer(this);
    return serializer.serialize();
}

void EllipseItem::deserialize(const QJsonObject &obj)
{
    EllipseDeserializer deserializer(this);
    deserializer.deserialize(obj);
}
