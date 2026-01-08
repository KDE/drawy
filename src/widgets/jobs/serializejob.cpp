/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "serializejob.hpp"
#include "drawy_debug.h"
#include "serializer/serializerutils.hpp"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
SerializeJob::SerializeJob(QObject *parent)
    : QObject{parent}
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
    obj[u"version"_s] = SerializerUtils::version();
    obj[u"offset_pos"_s] = SerializerUtils::toJson(mSerializeInfo.offsetPos);
    obj[u"zoom_factor"_s] = mSerializeInfo.zoomFactor;

    QJsonArray array;
    for (const auto &item : std::as_const(mSerializeInfo.items)) {
        array.push_back(item->serialize());
    }
    obj[u"items"_s] = array;

    Q_EMIT serializeDone(obj);
    deleteLater();
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
    d.space() << "offsetPos:" << t.offsetPos;
    d.space() << "zoomFactor:" << t.zoomFactor;
    d.space() << "items:" << t.items.count();
    return d;
}

#include "moc_serializejob.cpp"
