/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "autosavejobutil.hpp"
#include "drawy_autosave_debug.h"
#include <QDir>
#include <QFile>
#include <QStandardPaths>
using namespace Qt::Literals::StringLiterals;
void AutoSaveJobUtil::createAutoSaveStandardPath()
{
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
}

QString AutoSaveJobUtil::temporaryFileName()
{
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + u"/drawy.autosave"_s;
}

bool AutoSaveJobUtil::checkExistingAutoSaveFile()
{
    return QFile::exists(temporaryFileName());
}

void AutoSaveJobUtil::removeAutoSaveFile()
{
    if (!QFile::remove(temporaryFileName())) {
        qCWarning(DRAWY_AUTOSAVE_LOG) << "Impossible to remove autosave file" << temporaryFileName();
    }
}
