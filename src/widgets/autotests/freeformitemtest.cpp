/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "freeformitemtest.hpp"
#include "item/freeform.hpp"
#include <QTest>
QTEST_MAIN(FreeformItemTest)

FreeformItemTest::FreeformItemTest(QObject *parent)
    : QObject{parent}
{
}

void FreeformItemTest::shouldHaveDefaultValues()
{
    const FreeformItem i;
    QCOMPARE(i.type(), Item::Type::Freeform);
}

#include "moc_freeformitemtest.cpp"
