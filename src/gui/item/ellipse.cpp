// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ellipse.hpp"
#include "item/itemutils.hpp"
#include "serializer/ellipsedeserializer.hpp"
#include "serializer/ellipseserializer.hpp"
#include <QJsonObject>
#include <QPainterPath>

EllipseItem::EllipseItem()
{
}

void EllipseItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    prepareBackground(painter);
    painter.drawEllipse(QRectF(start() - offset, end() - offset));
}

bool EllipseItem::intersects(const QRectF &rect)
{
    if (!boundingBox().intersects(rect)) {
        return false;
    }

    QPainterPath path;
    path.addEllipse(QRectF{start(), end()});

    path = m_transform.map(path);

    if (isFilled()) {
        return path.intersects(rect);
    }

    return path.intersects(rect) && !path.contains(rect);
}

Item::FormType EllipseItem::formType() const
{
    return Item::FormType::Ellipse;
}

QJsonObject EllipseItem::serialize(int zorder) const
{
    const EllipseSerializer serializer(this);
    return serializer.serialize(zorder);
}

void EllipseItem::deserialize(const QJsonObject &obj)
{
    EllipseDeserializer deserializer(this);
    deserializer.deserialize(obj);
}
