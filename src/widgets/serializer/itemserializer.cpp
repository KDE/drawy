/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "itemserializer.hpp"
#include "item/item.hpp"
#include "item/itemutils.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;

ItemSerializer::ItemSerializer(const Item *item)
    : mItem(item)
{
}

ItemSerializer::~ItemSerializer() = default;

QJsonObject ItemSerializer::serialize(int zorder) const
{
    QJsonObject obj;

    obj[u"type"_s] = ItemUtils::convertItemTypeEnumToString(mItem->formType());
    obj[u"properties"_s] = toJson(mItem->properties());
    obj[u"id"_s] = QString::fromLatin1(mItem->id());
    obj[u"transform"_s] = toJson(mItem->transformObj());

    if (mItem->locked()) {
        obj[u"locked"_s] = true;
    }

    if (zorder != -1) {
        obj[u"zorder"_s] = QString::number(zorder);
    }

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

QJsonObject ItemSerializer::toJson(const QTransform &transform)
{
    QJsonObject result;

    result[u"m11"_s] = QJsonValue(transform.m11());
    result[u"m12"_s] = QJsonValue(transform.m12());
    result[u"m13"_s] = QJsonValue(transform.m13());
    result[u"m21"_s] = QJsonValue(transform.m21());
    result[u"m22"_s] = QJsonValue(transform.m22());
    result[u"m23"_s] = QJsonValue(transform.m23());
    result[u"m31"_s] = QJsonValue(transform.m31());
    result[u"m32"_s] = QJsonValue(transform.m32());
    result[u"m33"_s] = QJsonValue(transform.m33());

    return result;
}
