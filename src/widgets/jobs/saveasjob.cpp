/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "saveasjob.h"

SaveAsJob::SaveAsJob(QObject *parent)
    : QObject{parent}
{
}

SaveAsJob::~SaveAsJob() = default;

bool SaveAsJob::canStart() const
{
    // TODO
    return false;
}

void SaveAsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return;
    }
    // TODO
    deleteLater();
}

#include "moc_saveasjob.cpp"
