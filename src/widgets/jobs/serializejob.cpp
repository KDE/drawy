/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "serializejob.hpp"
#include "data-structures/quadtree.hpp"
#include "drawy_debug.h"
#include "page/page.hpp"
#include "serializer/serializerutils.hpp"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
SerializeJob::SerializeJob(ApplicationContext *context, QObject *parent)
    : QObject{parent}
    , mApplicationContext(context)
{
}

SerializeJob::~SerializeJob() = default;

bool SerializeJob::canStart() const
{
    // If in the future we need to check it.
    // For the moment return true
    return true;
}

void SerializeJob::start()
{
    if (!canStart()) {
        qCWarning(DRAWY_LOG) << "It's not valid";
        Q_EMIT serializeDone({});
        deleteLater();
        return;
    }
    serializeItems();
}

void SerializeJob::serializeItems()
{
    QJsonObject obj;
    obj[u"version"_s] = SerializerUtils::pageVersion();
    obj[u"current_page"_s] = mSerializeInfo.currentPage;
    obj[u"pages"_s] = serializePages();
    Q_EMIT serializeDone(obj);
    deleteLater();
}

QJsonArray SerializeJob::serializePages() const
{
    QJsonArray pageArray;
    for (const auto *page : std::as_const(mSerializeInfo.pages)) {
        QJsonObject pageObj;
        pageObj[u"offset_pos"_s] = SerializerUtils::toJson(page->offsetPos());
        pageObj[u"zoom_factor"_s] = page->zoomFactor();
        pageObj[u"page_name"_s] = page->name();

        QJsonArray array;
        const auto items = page->quadtree().getAllItems();
        for (const auto &item : items) {
            const int zorder = page->quadtree().zIndex(item);
            array.push_back(item->serialize(zorder));
        }
        pageObj[u"items"_s] = array;
        pageArray.append(pageObj);
    }
    return pageArray;
}

SerializeJob::SerializeInfo SerializeJob::serializeInfo() const
{
    return mSerializeInfo;
}

void SerializeJob::setSerializeInfo(const SerializeInfo &newSerializeInfo)
{
    mSerializeInfo = newSerializeInfo;
}

QDebug operator<<(QDebug d, const SerializeJob::SerializeInfo &t)
{
    d.space() << "currentPage:" << t.currentPage;
    d.space() << "Number of pages:" << t.pages.count();
    return d;
}

#include "moc_serializejob.cpp"
