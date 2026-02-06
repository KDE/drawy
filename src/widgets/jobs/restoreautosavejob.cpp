/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "restoreautosavejob.hpp"
#include "autosavejobutil.hpp"
#include "common/constants.hpp"
#include "context/applicationcontext.hpp"
#include "drawy_autosave_debug.h"
#include "drawyglobalconfig.h"
#include "jobs/loadjob.hpp"
#include "jobs/loadjobutil.hpp"
#include <KConfig>
#include <KConfigGroup>
#include <QFile>

RestoreAutoSaveJob::RestoreAutoSaveJob(ApplicationContext *context, QObject *parent)
    : QObject{parent}
    , m_context{context}
{
}

RestoreAutoSaveJob::~RestoreAutoSaveJob() = default;

void RestoreAutoSaveJob::start()
{
    if (DrawyGlobalConfig::self()->autoSaveEnabled()) {
        restoreFile();
    } else {
        removeAutoSaveFile();
    }
}

QWidget *RestoreAutoSaveJob::parentWidget() const
{
    return mParentWidget;
}

void RestoreAutoSaveJob::setParentWidget(QWidget *newParentWidget)
{
    mParentWidget = newParentWidget;
}

void RestoreAutoSaveJob::restoreFile()
{
    KConfig config{};
    KConfigGroup sessionGroup{&config, Common::configSession};

    const auto lastSavedFile{sessionGroup.readEntry(Common::configSessionLastSavedFile, u""_s)};
    const bool lastSavedFileModified{sessionGroup.readEntry(Common::configSessionLastSavedFileModified, false)};

    auto job = new LoadJob(m_context, this);
    job->setFileName(AutoSaveJobUtil::temporaryFileName());
    job->setIsAutoSave(true);

    connect(job, &LoadJob::loadDone, this, [this, lastSavedFile, lastSavedFileModified](const LoadJob::LoadInfo &info) {
        LoadJobUtil::loadFile(m_context, info);
        if (!lastSavedFile.isEmpty()) {
            m_context->setCurrentFileName(lastSavedFile);
            m_context->setCurrentFileModified(lastSavedFileModified);
        } else {
            // the last file was not saved which means we should mark it as modified
            m_context->setCurrentFileModified(true);
        }
    });

    job->start();
}

void RestoreAutoSaveJob::removeAutoSaveFile()
{
    QFile file(AutoSaveJobUtil::temporaryFileName());
    if (!file.remove()) {
        qCWarning(DRAWY_AUTOSAVE_LOG) << "Impossible to remove autosave file" << AutoSaveJobUtil::temporaryFileName();
    }
    Q_EMIT restoreDone();
    deleteLater();
}

#include "moc_restoreautosavejob.cpp"
