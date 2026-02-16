/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "groupdeserializer.hpp"
#include "item/group.hpp"
#include "item/itemutils.hpp"

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

    QList<std::shared_ptr<Item>> itemsToAdd;
    itemsToAdd.reserve(items.size());

    for (const auto &val : items) {
        const QJsonObject itemObj = val.toObject();
        const Item::FormType type{ItemUtils::convertItemTypeStringToEnum(ItemDeserializer::value(itemObj, u"type"_s).toString())};
        const std::shared_ptr<Item> item = ItemUtils::createItemFromType(type);
        item->deserialize(itemObj);

        itemsToAdd.append(item);
    }

    groupItem->setItems(itemsToAdd);
}
