/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "groupserializer.hpp"
#include "item/group.hpp"
#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
GroupSerializer::GroupSerializer(const GroupItem *item)
    : ItemSerializer(item)
{
}

GroupSerializer::~GroupSerializer() = default;

QJsonObject GroupSerializer::serialize() const
{
    const GroupItem *groupItem = static_cast<const GroupItem *>(mItem);
    const auto items = groupItem->items();
    QJsonArray array;
    for (const auto &i : items) {
        array.append(i->serialize());
    }
    QJsonObject obj = ItemSerializer::serialize();
    obj[u"items"_s] = array;
    return obj;
}
