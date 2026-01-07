/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "item/item.hpp"
#include "libdrawywidgets_private_export.h"
#include "properties/property.hpp"
#include <QJsonArray>
#include <QVector>
class QJsonObject;
class LIBDRAWYWIDGETS_TESTS_EXPORT ItemSerializer
{
public:
    explicit ItemSerializer(Item *item);
    virtual ~ItemSerializer();
    [[nodiscard]] virtual QJsonObject serialize() const;

protected:
    [[nodiscard]] static QJsonObject toJson(const Property &property);
    [[nodiscard]] static QJsonObject toJson(const QRectF &rect);
    [[nodiscard]] static QJsonObject toJson(const QPointF &point);

    Item *mItem = nullptr;

    template<typename T>
    static QJsonValue toJson(const T &val)
    {
        return QJsonValue(val);
    }

    template<typename T>
    static QJsonArray toJson(const QVector<T> &vector)
    {
        QJsonArray result;

        for (const T &el : vector) {
            result.push_back(toJson(el));
        }

        return result;
    }
};
