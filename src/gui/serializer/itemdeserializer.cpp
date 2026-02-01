/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "itemdeserializer.hpp"
#include "drawy_gui_debug.h"
#include "item/item.hpp"
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
    mItem->setLocked(obj[u"locked"_s].toBool(false));

    QJsonArray properties = array(value(obj, u"properties"_s));
    for (const auto &propertyValue : std::as_const(properties)) {
        const Property prop{createProperty(object(propertyValue))};
        mItem->setProperty(prop.type(), prop);
    }

    mItem->setTransform(toTransform(value(obj, u"transform"_s)));
}

QJsonArray ItemDeserializer::array(const QJsonValue &value)
{
    if (value.isUndefined() || !value.isArray()) {
        qCWarning(DRAWY_GUI_LOG) << "Value is not an array";
        return {};
    }

    return value.toArray();
}

QJsonValue ItemDeserializer::value(const QJsonObject &obj, const QString &key)
{
    if (!obj.contains(key)) {
        qCWarning(DRAWY_GUI_LOG) << "Object does not contain key: " << key;
        return {};
    }

    return obj.value(key);
}

QJsonObject ItemDeserializer::object(const QJsonValue &value)
{
    if (value.isUndefined() || !value.isObject()) {
        qCWarning(DRAWY_GUI_LOG) << "Value is not an object";
        return {};
    }

    return value.toObject();
}

Property ItemDeserializer::createProperty(const QJsonObject &obj)
{
    const Property::Type type{Property::convertStringToEnum(value(obj, u"type"_s).toString())};
    QVariant val{value(obj, u"value"_s).toVariant()};

    if (type == Property::Type::StrokeColor || type == Property::Type::BackgroundColor) {
        val = QColor(val.toString());
    } else if (type == Property::Type::StrokeWidth || type == Property::Type::Opacity || type == Property::Type::FontSize || type == Property::Type::EraserSize
               || type == Property::Type::ZOrder) {
        val = val.toInt();
    }

    return Property{val, type};
}

QPointF ItemDeserializer::toPointF(const QJsonValue &val)
{
    QJsonObject obj = object(val);

    if (obj.contains(u"x"_s) && obj.contains(u"y"_s)) {
        return QPointF{obj[u"x"].toDouble(), obj[u"y"].toDouble()};
    } else {
        qCWarning(DRAWY_GUI_LOG) << "Given point does not contain x and y properties";
        return {};
    }

    qCWarning(DRAWY_GUI_LOG) << "Given point value is not an object";
    return {};
}

QTransform ItemDeserializer::toTransform(const QJsonValue &val)
{
    const QJsonObject obj = object(val);

    const qreal m11 = value(obj, u"m11"_s).toDouble();
    const qreal m12 = value(obj, u"m12"_s).toDouble();
    const qreal m13 = value(obj, u"m13"_s).toDouble();
    const qreal m21 = value(obj, u"m21"_s).toDouble();
    const qreal m22 = value(obj, u"m22"_s).toDouble();
    const qreal m23 = value(obj, u"m23"_s).toDouble();
    const qreal m31 = value(obj, u"m31"_s).toDouble();
    const qreal m32 = value(obj, u"m32"_s).toDouble();
    const qreal m33 = value(obj, u"m33"_s).toDouble();

    return QTransform{m11, m12, m13, m21, m22, m23, m31, m32, m33};
}

QRectF ItemDeserializer::toRectF(const QJsonValue &val)
{
    QJsonObject obj = object(val);

    if (obj.contains(u"x"_s) && obj.contains(u"y"_s) && obj.contains(u"width"_s) && obj.contains(u"height"_s)) {
        return QRectF{obj[u"x"].toDouble(), obj[u"y"].toDouble(), obj[u"width"].toDouble(), obj[u"height"].toDouble()};
    } else {
        qCWarning(DRAWY_GUI_LOG) << "Given rect does not contain x, y, width and height properties";
        return {};
    }

    qCWarning(DRAWY_GUI_LOG) << "Given rect value is not an object";
    return {};
}
