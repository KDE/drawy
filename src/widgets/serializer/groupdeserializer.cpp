/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "groupdeserializer.hpp"
#include "item/group.hpp"
#include "jobs/deserializeutils.hpp"

#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
GroupDeserializer::GroupDeserializer(GroupItem *item)
    : ItemDeserializer(item)
{
}

GroupDeserializer::~GroupDeserializer() = default;

void GroupDeserializer::deserialize(const QJsonObject &obj)
{
    ItemDeserializer::deserialize(obj);

    GroupItem *groupItem = static_cast<GroupItem *>(mItem);

    const QJsonArray items = obj[u"items"_s].toArray();

    const QList<std::shared_ptr<Item>> itemsToAdd = DeserializeUtils::deserializeItems(items);
    groupItem->setItems(itemsToAdd);
}
