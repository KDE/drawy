/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "itemdeserializer.hpp"
#include "drawy_debug.h"
#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
ItemDeserializer::ItemDeserializer(Item *item)
    : mItem(item)
{
}

ItemDeserializer::~ItemDeserializer() = default;

void ItemDeserializer::deserialize(const QJsonObject &obj)
{
    mItem->setId(obj[u"id"_s].toString().toLatin1());
    mItem->setLocked(obj[u"id"_s].toBool(false));
    QJsonArray properties = array(value(obj, u"properties"_s));
    for (const auto &propertyValue : std::as_const(properties)) {
        Property prop{createProperty(object(propertyValue))};
        mItem->setProperty(prop.type(), prop);
    }
}

QJsonArray ItemDeserializer::array(const QJsonValue &value)
{
    if (value.isUndefined() || !value.isArray()) {
        qCWarning(DRAWY_LOG) << "Value is not an array";
        return {};
    }

    return value.toArray();
}

QJsonValue ItemDeserializer::value(const QJsonObject &obj, const QString &key)
{
    if (!obj.contains(key)) {
        qCWarning(DRAWY_LOG) << "Object does not contain key: " << key;
        return {};
    }

    return obj.value(key);
}

QJsonObject ItemDeserializer::object(const QJsonValue &value)
{
    if (value.isUndefined() || !value.isObject()) {
        qCWarning(DRAWY_LOG) << "Value is not an object";
        return {};
    }

    return value.toObject();
}

Property ItemDeserializer::createProperty(const QJsonObject &obj)
{
    const Property::Type type{Property::convertStringToEnum(value(obj, u"type"_s).toString())};
    const QVariant val{value(obj, u"value"_s).toVariant()};

    return Property{val, type};
}

QPointF ItemDeserializer::toPointF(const QJsonValue &val)
{
    QJsonObject obj = object(val);

    if (obj.contains(u"x"_s) && obj.contains(u"y"_s)) {
        return QPointF{obj[u"x"].toDouble(), obj[u"y"].toDouble()};
    } else {
        qCWarning(DRAWY_LOG) << "Given point does not contain x and y properties";
        return {};
    }

    qCWarning(DRAWY_LOG) << "Given point value is not an object";
    return {};
}
