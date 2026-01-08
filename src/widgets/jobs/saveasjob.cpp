/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "saveasjob.h"
#include "drawy_debug.h"
#include "serializejob.h"
#include <QDebug>
#include <QJsonObject>

SaveAsJob::SaveAsJob(QObject *parent)
    : QObject{parent}
{
}

SaveAsJob::~SaveAsJob() = default;

bool SaveAsJob::canStart() const
{
    return !mSaveAsInfo.filePath.isEmpty();
}

void SaveAsJob::start()
{
    if (!canStart()) {
        qCWarning(DRAWY_LOG) << "File path is not defined";
        Q_EMIT saveFileDone({});
        deleteLater();
        return;
    }
    auto job = new SerializeJob(this);
    const SerializeJob::SerializeInfo info{.offsetPos = mSaveAsInfo.offsetPos, .zoomFactor = mSaveAsInfo.zoomFactor, .items = mSaveAsInfo.items};
    job->setSerializeInfo(info);
    connect(job, &SerializeJob::serializeDone, this, &SaveAsJob::slotSerializeDone);
    job->start();
}

void SaveAsJob::slotSerializeDone(const QJsonObject &obj)
{
    Q_EMIT saveFileDone(obj);
    deleteLater();
}

SaveAsJob::SaveAsInfo SaveAsJob::saveAsInfo() const
{
    return mSaveAsInfo;
}

void SaveAsJob::setSaveAsInfo(const SaveAsInfo &newSaveAsInfo)
{
    mSaveAsInfo = newSaveAsInfo;
}

QDebug operator<<(QDebug d, const SaveAsJob::SaveAsInfo &t)
{
    d.space() << "filePath:" << t.filePath;
    d.space() << "offsetPos:" << t.offsetPos;
    d.space() << "zoomFactor:" << t.zoomFactor;
    d.space() << "items:" << t.items.count();
    return d;
}

#include "moc_saveasjob.cpp"
