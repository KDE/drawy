/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "rectangleitemtest.hpp"
#include "item/rectangle.hpp"
#include <QTest>
QTEST_MAIN(RectangleItemTest)

RectangleItemTest::RectangleItemTest(QObject *parent)
    : QObject{parent}
{
}

void RectangleItemTest::shouldHaveDefaultValues()
{
    const RectangleItem i;
    QCOMPARE(i.type(), Item::Type::Rectangle);
}

#include "moc_rectangleitemtest.cpp"
