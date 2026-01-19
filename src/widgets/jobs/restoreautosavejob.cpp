/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "restoreautosavejob.hpp"
#include "autosavejobutil.hpp"
#include "context/applicationcontext.hpp"
#include "drawy_autosave_debug.h"
#include "jobs/loadjob.hpp"
#include "jobs/loadjobutil.hpp"
#include <KMessageBox>
#include <QFile>

RestoreAutoSaveJob::RestoreAutoSaveJob(ApplicationContext *context, QObject *parent)
    : QObject{parent}
    , m_context{context}
{
}

RestoreAutoSaveJob::~RestoreAutoSaveJob() = default;

void RestoreAutoSaveJob::start()
{
    if (AutoSaveJobUtil::checkExistingAutoSaveFile()) {
        qCDebug(DRAWY_AUTOSAVE_LOG) << "Existing auto save file found";
        restoreFile();
    } else {
        Q_EMIT restoreDone();
        deleteLater();
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
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(mParentWidget,
                                           tr("Do you want to restore autosave file?"),
                                           tr("Restore"),
                                           KStandardGuiItem::ok(),
                                           KStandardGuiItem::cancel())) {
        auto job = new LoadJob(this);
        job->setFileName(AutoSaveJobUtil::temporaryFileName());
        connect(job, &LoadJob::loadDone, this, [this](const LoadJob::LoadInfo &info) {
            LoadJobUtil::loadFile(info);
            removeAutoSaveFile();
        });
        job->start();
    } else {
        removeAutoSaveFile();
    }
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
