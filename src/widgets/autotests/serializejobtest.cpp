/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "serializejobtest.hpp"
#include "jobs/serializejob.hpp"
#include <QSignalSpy>
#include <QTest>
QTEST_GUILESS_MAIN(SerializeJobTest)

SerializeJobTest::SerializeJobTest(QObject *parent)
    : QObject{parent}
{
}

void SerializeJobTest::shouldHaveDefaultValues()
{
    const SerializeJob::SerializeInfo info;
    QCOMPARE(info.offsetPos, {});
    QCOMPARE(info.zoomFactor, 1.0);
    QVERIFY(info.items.isEmpty());

    const SerializeJob j;
    QVERIFY(j.canStart());
}

#include "moc_serializejobtest.cpp"
