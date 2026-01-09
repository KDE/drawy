/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "deserializejob.hpp"
#include "drawy_debug.h"

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
}

#include "moc_deserializejob.cpp"
