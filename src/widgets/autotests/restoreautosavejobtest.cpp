/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "restoreautosavejobtest.hpp"
#include "jobs/restoreautosavejob.hpp"
#include <QTest>
QTEST_MAIN(RestoreAutoSaveJobTest)

RestoreAutoSaveJobTest::RestoreAutoSaveJobTest(QObject *parent)
    : QObject{parent}
{
}

void RestoreAutoSaveJobTest::shouldHaveDefaultValues()
{
    const RestoreAutoSaveJob j(nullptr);
    QVERIFY(!j.parentWidget());
}

#include "moc_restoreautosavejobtest.cpp"
