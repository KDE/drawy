/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "saveasjob.hpp"
#include "common/constants.hpp"
#include "context/applicationcontext.hpp"
#include "drawy_debug.h"
#include "serializejob.hpp"
#include <KConfig>
#include <KConfigGroup>
#include <QDebug>
#include <QJsonObject>

using namespace Qt::StringLiterals;
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

    auto context{ApplicationContext::instance()};

    KConfig config{Common::configName};
    KConfigGroup sessionGroup{&config, Common::configSession};

    // only update modified status if it's not the autosaved file
    // the autosave file is just a backup file
    if (!mSaveAsInfo.isAutoSave) {
        context->setCurrentFileModified(false);
        context->setCurrentFileName(mSaveAsInfo.filePath);
    }

    if (!context->fileNeedsName()) {
        sessionGroup.writeEntry(Common::configSessionLastSavedFile, context->currentFileName());
        if (context->currentFileModified()) {
            sessionGroup.writeEntry(Common::configSessionLastSavedFileModified, true);
            if (mSaveAsInfo.isAutoSave) {
                sessionGroup.writeEntry(Common::configSessionLastSavedFileModified, true);
            } else {
                sessionGroup.writeEntry(Common::configSessionLastSavedFileModified, false);
            }
        } else {
            sessionGroup.writeEntry(Common::configSessionLastSavedFileModified, false);
        }
    } else {
        sessionGroup.writeEntry(Common::configSessionLastSavedFile, u""_s);
    }

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
