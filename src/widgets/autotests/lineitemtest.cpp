/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "lineitemtest.hpp"
#include "item/line.hpp"
#include <QTest>
QTEST_MAIN(LineItemTest)

LineItemTest::LineItemTest(QObject *parent)
    : QObject{parent}
{
}

void LineItemTest::shouldHaveDefaultValues()
{
    const LineItem i;
    QCOMPARE(i.type(), Item::Type::Line);
}

#include "moc_lineitemtest.cpp"
