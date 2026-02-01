/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "libdrawygui_export.h"
#include "properties/property.hpp"
class Item;
class QJsonObject;
class LIBDRAWYGUI_EXPORT ItemDeserializer
{
public:
    explicit ItemDeserializer(Item *item);
    virtual ~ItemDeserializer();

    virtual void deserialize(const QJsonObject &obj);

    [[nodiscard]] static QJsonArray array(const QJsonValue &value);
    [[nodiscard]] static QJsonValue value(const QJsonObject &obj, const QString &key);
    [[nodiscard]] static QJsonObject object(const QJsonValue &value);
    [[nodiscard]] static Property createProperty(const QJsonObject &obj);
    [[nodiscard]] static QPointF toPointF(const QJsonValue &val);
    [[nodiscard]] static QTransform toTransform(const QJsonValue &val);
    [[nodiscard]] static QRectF toRectF(const QJsonValue &val);

    Item *const mItem;
};
