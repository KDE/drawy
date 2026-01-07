/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "polygonserializer.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
PolygonSerializer::PolygonSerializer(const std::shared_ptr<PolygonItem> &item)
    : ItemSerializer(item)
{
}

PolygonSerializer::~PolygonSerializer() = default;

QJsonObject PolygonSerializer::serialize() const
{
    QJsonObject obj = ItemSerializer::serialize();
    std::shared_ptr<PolygonItem> polygon{std::dynamic_pointer_cast<PolygonItem>(mItem)};
    obj[u"start"_s] = toJson(polygon->start());
    obj[u"end"_s] = toJson(polygon->end());
    return obj;
}
