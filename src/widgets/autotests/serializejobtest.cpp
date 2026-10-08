/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "serializejobtest.hpp"
#include "jobs/serializejob.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(SerializeJobTest)

SerializeJobTest::SerializeJobTest(QObject *parent)
    : QObject{parent}
{
}

void SerializeJobTest::shouldHaveDefaultValues()
{
    const SerializeJob::SerializeInfo info;
    QCOMPARE(info.currentPage, 0);
    QVERIFY(info.pages.isEmpty());

    const SerializeJob j(nullptr);
    QVERIFY(j.canStart());
}

#include "moc_serializejobtest.cpp"
