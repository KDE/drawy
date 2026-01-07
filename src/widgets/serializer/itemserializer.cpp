/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "itemserializer.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;

ItemSerializer::ItemSerializer(Item *item)
    : mItem(item)
{
}

ItemSerializer::~ItemSerializer() = default;

QJsonObject ItemSerializer::serialize() const
{
    QJsonObject obj;

    obj[u"type"_s] = Item::convertEnumToString(mItem->type());
    obj[u"bounding_box"_s] = toJson(mItem->boundingBox());
    obj[u"bounding_box_padding"_s] = QJsonValue(mItem->boundingBoxPadding());
    obj[u"properties"_s] = toJson(mItem->properties());
    return obj;
}

QJsonObject ItemSerializer::toJson(const Property &property)
{
    QJsonObject result;

    result[u"type"_s] = Property::convertEnumToString(property.type());
    result[u"value"_s] = QJsonValue::fromVariant(property.variant());

    return result;
}

QJsonObject ItemSerializer::toJson(const QRectF &rect)
{
    QJsonObject result;
    result[u"x"_s] = QJsonValue(rect.x());
    result[u"y"_s] = QJsonValue(rect.y());
    result[u"width"_s] = QJsonValue(rect.width());
    result[u"height"_s] = QJsonValue(rect.height());

    return result;
}

QJsonObject ItemSerializer::toJson(const QPointF &point)
{
    QJsonObject result;
    result[u"x"_s] = QJsonValue(point.x());
    result[u"y"_s] = QJsonValue(point.y());

    return result;
}
