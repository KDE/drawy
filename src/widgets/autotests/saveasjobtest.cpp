/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "saveasjobtest.h"
#include "jobs/saveasjob.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(SaveAsJobTest)
SaveAsJobTest::SaveAsJobTest(QObject *parent)
    : QObject{parent}
{
}

void SaveAsJobTest::shouldHaveDefaultValues()
{
    const SaveAsJob::SaveAsInfo info;
    QVERIFY(info.filePath.isEmpty());
    QCOMPARE(info.offsetPos, {});
    QCOMPARE(info.zoomFactor, 0);
    QVERIFY(info.items.isEmpty());

    const SaveAsJob j;
    QVERIFY(!j.canStart());
}

#include "moc_saveasjobtest.cpp"
