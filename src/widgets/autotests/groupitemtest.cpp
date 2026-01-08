/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "groupitemtest.hpp"
#include "item/group.hpp"
#include <QTest>
QTEST_MAIN(GroupItemTest)

GroupItemTest::GroupItemTest(QObject *parent)
    : QObject{parent}
{
}

void GroupItemTest::shouldHaveDefaultValues()
{
    const GroupItem i;
    QCOMPARE(i.type(), Item::Type::Group);
}

#include "moc_groupitemtest.cpp"
