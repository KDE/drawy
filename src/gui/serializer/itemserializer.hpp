/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "libdrawygui_export.h"
#include "properties/property.hpp"
#include <QJsonArray>
#include <QList>
class QJsonObject;
class Item;
class LIBDRAWYGUI_EXPORT ItemSerializer
{
public:
    explicit ItemSerializer(const Item *item);
    virtual ~ItemSerializer();
    [[nodiscard]] virtual QJsonObject serialize(int zorder) const;

protected:
    [[nodiscard]] static QJsonObject toJson(const Property &property);
    [[nodiscard]] static QJsonObject toJson(const QRectF &rect);
    [[nodiscard]] static QJsonObject toJson(const QPointF &point);
    [[nodiscard]] static QJsonObject toJson(const QTransform &transform);

    const Item *mItem = nullptr;

    template<typename T>
    static QJsonValue toJson(const T &val)
    {
        return QJsonValue(val);
    }

    template<typename T>
    static QJsonArray toJson(const QList<T> &vector)
    {
        QJsonArray result;

        for (const T &el : vector) {
            result.push_back(toJson(el));
        }

        return result;
    }
};
