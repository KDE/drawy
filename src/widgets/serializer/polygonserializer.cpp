/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "polygonserializer.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
PolygonSerializer::PolygonSerializer(PolygonItem *item)
    : ItemSerializer(item)
{
}

PolygonSerializer::~PolygonSerializer() = default;

QJsonObject PolygonSerializer::serialize() const
{
    QJsonObject obj = ItemSerializer::serialize();
    PolygonItem *polygon = dynamic_cast<PolygonItem *>(mItem);
    obj[u"start"_s] = toJson(polygon->start());
    obj[u"end"_s] = toJson(polygon->end());
    return obj;
}
