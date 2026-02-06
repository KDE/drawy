// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mime/drawymimehandler.hpp"
#include "common/constants.hpp"
#include "item/item.hpp"
#include "item/itemutils.hpp"
#include "serializer/itemdeserializer.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMimeData>
#include <memory>

QList<std::shared_ptr<Item>> DrawyMimeHandler::tryReadData(const QMimeData &mimeData)
{
    if (!mimeData.hasFormat(Common::drawyClipboardMime)) {
        return {};
    }

    const QJsonDocument data{QJsonDocument::fromJson(mimeData.data(Common::drawyClipboardMime))};

    if (!data.isArray()) {
        return {};
    }

    QList<std::shared_ptr<Item>> items;
    items.reserve(data.array().count());

    for (const auto &v : data.array()) {
        const QJsonObject itemObj = ItemDeserializer::object(v);
        const Item::FormType type{ItemUtils::convertItemTypeStringToEnum(ItemDeserializer::value(itemObj, u"type"_s).toString())};
        const std::shared_ptr<Item> item = ItemUtils::createItemFromType(type);
        item->deserialize(itemObj);
        items.append(item);
    }

    return items;
}

void DrawyMimeHandler::contributeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems)
{
    QJsonArray array;

    for (const auto &item : std::as_const(selectedItems)) {
        // TODO: fetch zorder from application context... if that even makes sense here
        array.push_back(item->serialize(-1));
    }

    const QJsonDocument doc{array};
    const auto data{doc.toJson(QJsonDocument::Compact)};

    mimeData.setData(Common::drawyClipboardMime, data);
}
