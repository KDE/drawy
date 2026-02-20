/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "deserializejob.hpp"
#include "drawy_debug.h"
#include "item/itemutils.hpp"
#include "serializer/itemdeserializer.hpp"
#include <QJsonArray>
using namespace Qt::Literals::StringLiterals;
DeserializeJob::DeserializeJob(QObject *parent)
    : QObject{parent}
{
}

DeserializeJob::~DeserializeJob() = default;

bool DeserializeJob::canStart() const
{
    return !mJsonObject.isEmpty();
}

void DeserializeJob::start()
{
    if (!canStart()) {
        qCWarning(DRAWY_LOG) << "JsonObject is not valid";
        deleteLater();
        return;
    }
    deserializeItems();
}

QJsonObject DeserializeJob::jsonObject() const
{
    return mJsonObject;
}

void DeserializeJob::setJsonObject(const QJsonObject &newJsonObject)
{
    mJsonObject = newJsonObject;
}

void DeserializeJob::deserializeItems()
{
    QList<std::shared_ptr<Item>> items;
    const QJsonArray itemsArray = ItemDeserializer::array(ItemDeserializer::value(mJsonObject, u"items"_s));
    for (const auto &v : itemsArray) {
        const QJsonObject itemObj = ItemDeserializer::object(v);
        const Item::FormType type{ItemUtils::convertItemTypeStringToEnum(ItemDeserializer::value(itemObj, u"type"_s).toString())};
        const std::shared_ptr<Item> item = ItemUtils::createItemFromType(type);
        if (item) {
            item->deserialize(itemObj);
            items.append(item);
        }
    }

    const qreal zoomFactor = ItemDeserializer::value(mJsonObject, u"zoom_factor"_s).toDouble();

    const QPointF offsetPos = ItemDeserializer::toPointF(ItemDeserializer::value(mJsonObject, u"offset_pos"_s));
    const DeserializeInfo info{
        .offsetPos = offsetPos,
        .zoomFactor = zoomFactor,
        .items = items,
    };
    Q_EMIT deserializeDone(info);
}

#include "moc_deserializejob.cpp"
