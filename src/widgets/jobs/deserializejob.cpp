/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "deserializejob.hpp"
#include "drawy_debug.h"
#include "item/arrow.hpp"
#include "item/ellipse.hpp"
#include "item/freeform.hpp"
#include "item/group.hpp"
#include "item/itemutils.hpp"
#include "item/line.hpp"
#include "item/rectangle.hpp"
#include "item/text.hpp"
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
    QJsonArray itemsArray = ItemDeserializer::array(ItemDeserializer::value(mJsonObject, u"items"_s));
    for (const QJsonValueRef &v : itemsArray) {
        const QJsonObject itemObj = ItemDeserializer::object(v);
        const Item::Type type{ItemUtils::convertItemTypeStringToEnum(ItemDeserializer::value(itemObj, u"type"_s).toString())};
        std::shared_ptr<Item> item = createItem(type);
        item->deserialize(itemObj);
        items.append(item);
    }

    const qreal zoomFactor = ItemDeserializer::value(mJsonObject, u"zoom_factor"_s).toDouble();

    const QPointF offsetPos = ItemDeserializer::toPointF(ItemDeserializer::value(mJsonObject, u"offset_pos"_s));
    const DeserializeInfo info{
        .offsetPos = offsetPos,
        .zoomOffset = zoomFactor,
        .items = items,
    };
    Q_EMIT deserializeDone(info);
}

std::shared_ptr<Item> DeserializeJob::createItem(Item::Type type)
{
    std::shared_ptr<Item> item;
    switch (type) {
    case Item::Type::Freeform: {
        item = std::make_shared<FreeformItem>();
        break;
    }
    case Item::Type::Rectangle: {
        item = std::make_shared<RectangleItem>();
        break;
    }
    case Item::Type::Line: {
        item = std::make_shared<LineItem>();
        break;
    }
    case Item::Type::Arrow: {
        item = std::make_shared<ArrowItem>();
        break;
    }
    case Item::Type::Ellipse: {
        item = std::make_shared<EllipseItem>();
        break;
    }
    case Item::Type::Text: {
        item = std::make_shared<TextItem>();
        break;
    }
    case Item::Type::Group: {
        item = std::make_shared<GroupItem>();
        break;
    }
    case Item::Type::Invalid:
        Q_ASSERT(false);
        break;
    }
    return item;
}

#include "moc_deserializejob.cpp"
