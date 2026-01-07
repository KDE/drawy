/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "serializejob.h"

SerializeJob::SerializeJob(QObject *parent)
    : QObject{parent}
{
}

SerializeJob::~SerializeJob() = default;

bool SerializeJob::canStart() const
{
    // TODO
    return false;
}

void SerializeJob::start()
{
    if (!canStart()) {
        deleteLater();
        return;
    }
    // TODO
    deleteLater();
}

#include "moc_serializejob.cpp"
