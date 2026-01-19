/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "autosavejobutiltest.hpp"
#include "jobs/autosavejobutil.hpp"
#include <QStandardPaths>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(AutoSaveJobUtilTest)

AutoSaveJobUtilTest::AutoSaveJobUtilTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void AutoSaveJobUtilTest::shouldHaveDefaultFileName()
{
    QCOMPARE(AutoSaveJobUtil::temporaryFileName(), QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + u"/drawy.autosave"_s);
}

#include "moc_autosavejobutiltest.cpp"
