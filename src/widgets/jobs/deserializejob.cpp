/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "deserializejob.hpp"
#include "drawy_debug.h"
#include "serializer/deserializeutils.hpp"
#include "serializer/itemdeserializer.hpp"
#include "serializer/serializerutils.hpp"
#include <KLocalizedString>
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
        Q_EMIT deserializeFailed();
        deleteLater();
        return;
    }
    const int version = mJsonObject["version"_L1].toInt();
    if (version == SerializerUtils::pageVersion()) {
        deserializePages();
    } else if (version >= static_cast<int>(SerializerUtils::Version::Initial) && version <= SerializerUtils::version()) {
        deserializeItems();
    } else {
        qCWarning(DRAWY_LOG) << "Invalid file version:" << version;
        Q_EMIT deserializeFailed();
        deleteLater();
    }
}

QJsonObject DeserializeJob::jsonObject() const
{
    return mJsonObject;
}

void DeserializeJob::setJsonObject(const QJsonObject &newJsonObject)
{
    mJsonObject = newJsonObject;
}

void DeserializeJob::deserializePages()
{
    LoadJobUtil::DeserializeInfo info;
    info.currentPage = mJsonObject["current_page"_L1].toInt();
    const QJsonArray pagesObj = mJsonObject["pages"_L1].toArray();
    QList<LoadJobUtil::DeserializePageInfo> pages;
    for (const auto &page : pagesObj) {
        const QJsonObject pageObj = page.toObject();
        LoadJobUtil::DeserializePageInfo pageInfo;

        pageInfo.zoomFactor = ItemDeserializer::value(pageObj, u"zoom_factor"_s).toDouble();
        pageInfo.offsetPos = ItemDeserializer::toPointF(ItemDeserializer::value(pageObj, u"offset_pos"_s));
        const QJsonArray itemsArray = ItemDeserializer::array(ItemDeserializer::value(pageObj, u"items"_s));
        pageInfo.items = DeserializeUtils::deserializeItems(itemsArray);
        pageInfo.name = pageObj[u"page_name"_s].toString();
        pages.append(std::move(pageInfo));
    }
    info.pages = std::move(pages);
    Q_EMIT deserializeDone(info);
}

void DeserializeJob::deserializeItems()
{
    LoadJobUtil::DeserializeInfo info;
    info.currentPage = 0;
    QList<LoadJobUtil::DeserializePageInfo> pages;
    LoadJobUtil::DeserializePageInfo pageInfo;
    const QJsonArray itemsArray = ItemDeserializer::array(ItemDeserializer::value(mJsonObject, u"items"_s));
    pageInfo.items = DeserializeUtils::deserializeItems(itemsArray);
    pageInfo.zoomFactor = ItemDeserializer::value(mJsonObject, u"zoom_factor"_s).toDouble();
    pageInfo.offsetPos = ItemDeserializer::toPointF(ItemDeserializer::value(mJsonObject, u"offset_pos"_s));
    pageInfo.name = i18n("Page 1");
    pages.append(std::move(pageInfo));
    info.pages = std::move(pages);
    Q_EMIT deserializeDone(info);
}

#include "moc_deserializejob.cpp"
