/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "loadjob.hpp"
#include "common/utils/compression.hpp"
#include "drawy_debug.h"
#include <QFile>
LoadJob::LoadJob(QObject *parent)
    : QObject{parent}
{
}

LoadJob::~LoadJob() = default;

bool LoadJob::canStart() const
{
    return !mFileName.isEmpty();
}

void LoadJob::start()
{
    if (!canStart()) {
        qCWarning(DRAWY_LOG) << "File path is not defined";
        deleteLater();
        return;
    }

    QFile file(mFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(DRAWY_LOG) << "Failed to open file:" << file.errorString();
        deleteLater();
        return;
    }

    const QByteArray compressedByteArray = file.readAll();
    file.close();

    const QByteArray byteArray = Common::Utils::Compression::decompressData(compressedByteArray);
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(byteArray, &parseError);
    if (doc.isNull() || !doc.isObject()) {
        qCWarning(DRAWY_LOG) << "JSON parse failed:" << parseError.errorString() << "offset:" << parseError.offset;
        deleteLater();
        return;
    }

    const QJsonObject docObj = doc.object();
    auto job = new DeserializeJob(this);
    job->setJsonObject(docObj);
    connect(job, &DeserializeJob::deserializeDone, this, &LoadJob::slotDeserializeDone);
    job->start();
}

void LoadJob::slotDeserializeDone(const DeserializeJob::DeserializeInfo &info)
{
    const LoadJob::LoadInfo loadInfo{
        .offsetPos = info.offsetPos,
        .zoomFactor = info.zoomFactor,
        .items = info.items,
    };
    Q_EMIT loadDone(loadInfo);
    deleteLater();
}

QString LoadJob::fileName() const
{
    return mFileName;
}

void LoadJob::setFileName(const QString &newFileName)
{
    mFileName = newFileName;
}

#include "moc_loadjob.cpp"
