/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "customelementsutils.hpp"
#include <QDir>
#include <QStandardPaths>
using namespace Qt::Literals::StringLiterals;
QString CustomElementsUtils::customElementsFilePath()
{
    const QString customElementsPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir().mkpath(customElementsPath);
    return customElementsPath + u"/custom-elements"_s;
}
