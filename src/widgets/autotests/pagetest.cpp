/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "pagetest.hpp"
#include "page/page.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(PageTest)

PageTest::PageTest(QObject *parent)
    : QObject{parent}
{
}

void PageTest::shouldHaveDefaultValues()
{
    Page p;
    QVERIFY(p.name().isEmpty());
}
