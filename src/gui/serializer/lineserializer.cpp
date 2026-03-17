/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "lineserializer.hpp"
#include "item/line.hpp"
#include "itemserializer.hpp"
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
LineSerializer::LineSerializer(const LineItem *item)
    : ItemSerializer(item)
{
}

LineSerializer::~LineSerializer() = default;

QJsonObject LineSerializer::serialize(int zorder) const
{
    QJsonObject obj = ItemSerializer::serialize(zorder);
    const LineItem *polygon = dynamic_cast<const LineItem *>(mItem);
    obj[u"start"_s] = toJson(polygon->start());
    obj[u"end"_s] = toJson(polygon->end());
    return obj;
}
