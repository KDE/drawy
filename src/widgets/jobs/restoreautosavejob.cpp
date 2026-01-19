/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "restoreautosavejob.hpp"
#include "autosavejobutil.hpp"
#include "drawy_autosave_debug.h"

RestoreAutoSaveJob::RestoreAutoSaveJob(QObject *parent)
    : QObject{parent}
{
}

RestoreAutoSaveJob::~RestoreAutoSaveJob() = default;

void RestoreAutoSaveJob::start()
{
    if (AutoSaveJobUtil::checkExistingAutoSaveFile()) {
        qCDebug(DRAWY_AUTOSAVE_LOG) << "Existing auto save file";
    } else {
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
