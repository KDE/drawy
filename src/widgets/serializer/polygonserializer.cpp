/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "polygonserializer.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
PolygonSerializer::PolygonSerializer(const PolygonItem *item)
    : ItemSerializer(item)
{
}

PolygonSerializer::~PolygonSerializer() = default;

QJsonObject PolygonSerializer::serialize() const
{
    QJsonObject obj = ItemSerializer::serialize();
    const PolygonItem *polygon = dynamic_cast<const PolygonItem *>(mItem);
    obj[u"start"_s] = toJson(polygon->start());
    obj[u"end"_s] = toJson(polygon->end());
    return obj;
}
