/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "autosavejobutil.hpp"
#include "drawy_autosave_debug.h"
#include "drawyglobalconfig.h"
#include <QDir>
#include <QFile>

using namespace Qt::Literals::StringLiterals;
void AutoSaveJobUtil::createAutoSaveStandardPath()
{
    QDir().mkpath(QFileInfo(DrawyGlobalConfig::self()->path()).absolutePath());
}

QString AutoSaveJobUtil::temporaryFileName()
{
    return DrawyGlobalConfig::self()->path();
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
