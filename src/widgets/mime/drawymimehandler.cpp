// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mime/drawymimehandler.hpp"
#include "common/constants.hpp"
#include "item/item.hpp"
#include "serializer/deserializeutils.hpp"
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

    const QList<std::shared_ptr<Item>> items = DeserializeUtils::deserializeItems(data.array());
    return items;
}

void DrawyMimeHandler::contributeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems)
{
    QJsonArray array;

    for (const auto &item : std::as_const(selectedItems)) {
        // TODO: fetch zorder from application context... if that even makes sense
        // here
        array.push_back(item->serialize(-1));
    }

    const QJsonDocument doc{array};
    const auto data{doc.toJson(QJsonDocument::Compact)};

    mimeData.setData(Common::drawyClipboardMime, data);
}
