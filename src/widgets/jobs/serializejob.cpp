/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "serializejob.h"
#include "drawy_debug.h"
#include "serializer/serializerutils.h"
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
