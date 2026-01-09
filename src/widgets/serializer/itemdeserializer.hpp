/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "item/item.hpp"
class QJsonObject;
class ItemDeserializer
{
public:
    explicit ItemDeserializer(Item *item);
    virtual ~ItemDeserializer();

    virtual void deserialize(const QJsonObject &obj);

protected:
    [[nodiscard]] static QJsonArray array(const QJsonValue &value);
    [[nodiscard]] static QJsonValue value(const QJsonObject &obj, const QString &key);
    [[nodiscard]] static QJsonObject object(const QJsonValue &value);
    [[nodiscard]] static Property createProperty(const QJsonObject &obj);
    [[nodiscard]] static QPointF toPointF(const QJsonValue &val);

    Item *const mItem;
};
