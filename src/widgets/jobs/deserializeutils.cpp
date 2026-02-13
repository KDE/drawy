/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "deserializeutils.hpp"
#include "item/itemutils.hpp"
#include "pluginform/pluginformmanager.hpp"
#include "pluginform/pluginformutils.hpp"
#include "serializer/itemdeserializer.hpp"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

QList<std::shared_ptr<Item>> DeserializeUtils::deserializeItems(const QJsonArray &items)
{
    QList<std::shared_ptr<Item>> itemsToAdd;
    itemsToAdd.reserve(items.size());

    for (const auto &val : items) {
        const QJsonObject itemObj = val.toObject();
        const Item::FormType type{ItemUtils::convertItemTypeStringToEnum(ItemDeserializer::value(itemObj, u"type"_s).toString())};
        if (type == Item::FormType::Custom) {
            QString pluginName;
            QString customObjectName;
            PluginFormUtils::deserializePluginForm(itemObj, pluginName, customObjectName);
            // qDebug() << " pluginName " << pluginName << " customObjectName " << customObjectName;
            const std::shared_ptr<Item> item = PluginFormManager::self()->createItem(pluginName, customObjectName);
            if (item) {
                item->deserialize(itemObj);
                itemsToAdd.append(item);
            }
        } else {
            const std::shared_ptr<Item> item = ItemUtils::createItemFromType(type);
            if (item) {
                item->deserialize(itemObj);
                itemsToAdd.append(item);
            }
        }
    }
    return itemsToAdd;
}
