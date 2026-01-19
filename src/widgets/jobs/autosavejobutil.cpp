/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "autosavejobutil.hpp"
#include <QFile>
#include <QStandardPaths>
using namespace Qt::Literals::StringLiterals;
QString AutoSaveJobUtil::temporaryFileName()
{
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + u"/drawy.autosave"_s;
}

bool AutoSaveJobUtil::checkExistingAutoSaveFile()
{
    return QFile::exists(temporaryFileName());
}
