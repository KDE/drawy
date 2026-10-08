/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "loadjob.hpp"
#include "common/utils/compression.hpp"
#include "context/applicationcontext.hpp"
#include "drawy_debug.h"
#include "jobs/deserializejob.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
LoadJob::LoadJob(ApplicationContext *context, QObject *parent)
    : QObject{parent}
    , mApplicationContext(context)
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
        Q_EMIT loadFailed();
        deleteLater();
        return;
    }

    QFile file(mFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(DRAWY_LOG) << "Failed to open file:" << file.errorString();
        Q_EMIT loadFailed();
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
        Q_EMIT loadFailed();
        deleteLater();
        return;
    }

    const QJsonObject docObj = doc.object();
    auto job = new DeserializeJob(this);
    job->setJsonObject(docObj);
    connect(job, &DeserializeJob::deserializeDone, this, &LoadJob::slotDeserializeDone);
    connect(job, &DeserializeJob::deserializeFailed, this, &LoadJob::slotDeserializeFailed);

    job->start();
}

void LoadJob::slotDeserializeFailed()
{
    Q_EMIT loadFailed();
    deleteLater();
}

void LoadJob::slotDeserializeDone(const LoadJobUtil::DeserializeInfo &info)
{
    Q_EMIT loadDone(info);

    if (!mIsAutoSave) {
        if (mApplicationContext) {
            mApplicationContext->setCurrentFileModified(false);
            mApplicationContext->setCurrentFileName(mFileName);
        }
    }

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

void LoadJob::setIsAutoSave(bool value)
{
    mIsAutoSave = value;
}

#include "moc_loadjob.cpp"
