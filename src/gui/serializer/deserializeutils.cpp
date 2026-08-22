/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "deserializeutils.hpp"
#include "drawy_gui_debug.h"
#include "item/itemutils.hpp"
#include "pluginform/pluginformmanager.hpp"
#include "pluginform/pluginformutils.hpp"
#include "serializer/itemdeserializer.hpp"
#include <QJsonArray>
#include <QJsonObject>
#include <utility>

using namespace Qt::Literals::StringLiterals;

QList<std::shared_ptr<Item>> DeserializeUtils::deserializeItems(const QJsonArray &items)
{
    QList<std::shared_ptr<Item>> itemsToAdd;
    itemsToAdd.reserve(items.size());

    for (const auto &val : items) {
        const QJsonObject itemObj = val.toObject();
        std::shared_ptr<Item> item = DeserializeUtils::deserializeItem(itemObj);
        if (item) {
            itemsToAdd.append(std::move(item));
        } else {
            qCWarning(DRAWY_GUI_LOG) << "Impossible to deserialize item";
        }
    }
    return itemsToAdd;
}

std::shared_ptr<Item> DeserializeUtils::deserializeItem(const QJsonObject &itemObj)
{
    std::shared_ptr<Item> item;
    const Item::FormType type{ItemUtils::convertItemTypeStringToEnum(ItemDeserializer::value(itemObj, u"type"_s).toString())};
    if (type == Item::FormType::Custom) {
        QString pluginName;
        QString customObjectName;
        PluginFormUtils::deserializePluginForm(itemObj, pluginName, customObjectName);
        // qDebug() << " pluginName " << pluginName << " customObjectName " << customObjectName;
        item = PluginFormManager::self()->createItem(pluginName, customObjectName);
        if (item) {
            item->deserialize(itemObj);
        } else {
            qCWarning(DRAWY_GUI_LOG) << "Impossible to custom deserialize item: customObjectName" << customObjectName;
        }
    } else {
        item = ItemUtils::createItemFromType(type);
        if (item) {
            item->deserialize(itemObj);
        } else {
            qCWarning(DRAWY_GUI_LOG) << "Impossible to deserialize item";
        }
    }
    return item;
}
