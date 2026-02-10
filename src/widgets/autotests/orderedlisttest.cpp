/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "orderedlisttest.hpp"
#include "data-structures/orderedlist.hpp"
#include "item/rectangle.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(OrderedListTest)

OrderedListTest::OrderedListTest(QObject *parent)
    : QObject{parent}
{
}

void OrderedListTest::shouldHaveDefaultValues()
{
    const OrderedList list;
    QVERIFY(list.itemList().empty());
}

void OrderedListTest::shouldContainsItems()
{
    const OrderedList list;
    auto item = std::make_shared<RectangleItem>();
    QVERIFY(!list.hasItem(item));
}

void OrderedListTest::shouldCheckMove()
{
    OrderedList list;
    auto item = std::make_shared<RectangleItem>();
    QVERIFY(!list.hasItem(item));

    // Test that it doesn't crash when we have no item
    QVERIFY(!list.canMoveBackwards(item));
    QVERIFY(!list.canMoveForwards(item));

    list.insert(item);
    QVERIFY(!list.canMoveBackwards(item));
    QVERIFY(!list.canMoveForwards(item));

    // Verify item2
    auto item2 = std::make_shared<RectangleItem>();
    list.insert(item2);
    QVERIFY(list.canMoveBackwards(item2));
    QVERIFY(!list.canMoveForwards(item2));

    // Verify item3
    auto item3 = std::make_shared<RectangleItem>();
    list.insert(item3);
    QVERIFY(list.canMoveBackwards(item3));
    QVERIFY(!list.canMoveForwards(item3));

    // Now revverify item2
    QVERIFY(list.canMoveBackwards(item2));
    QVERIFY(list.canMoveForwards(item2));
}

void OrderedListTest::shouldRemoveItems()
{
    OrderedList list;
    QVERIFY(list.itemList().empty());
    auto item = std::make_shared<RectangleItem>();
    list.insert(item);
    QVERIFY(list.hasItem(item));
    QVERIFY(!list.itemList().empty());
    QCOMPARE(list.itemList().size(), 1);

    list.remove(item);
    QVERIFY(list.itemList().empty());
    QVERIFY(!list.hasItem(item));
}

void OrderedListTest::shouldAddItems()
{
    OrderedList list;
    QVERIFY(list.itemList().empty());
    auto item = std::make_shared<RectangleItem>();
    list.insert(item);
    QVERIFY(list.hasItem(item));
    QCOMPARE(list.zIndex(item), 0);

    auto item2 = std::make_shared<RectangleItem>();
    list.insert(item2);
    QVERIFY(list.hasItem(item2));
    QCOMPARE(list.zIndex(item2), 1);

    auto item3 = std::make_shared<RectangleItem>();
    list.insert(item3);
    QVERIFY(list.hasItem(item3));
    QCOMPARE(list.zIndex(item3), 2);
    QCOMPARE(list.itemList().size(), 3);
}

void OrderedListTest::shouldUpdateList()
{
    OrderedList list;
    QVERIFY(list.itemList().empty());
    auto item = std::make_shared<RectangleItem>();
    list.insert(item);
    QVERIFY(list.hasItem(item));
    QCOMPARE(list.zIndex(item), 0);

    auto item2 = std::make_shared<RectangleItem>();
    list.insert(item2);
    QVERIFY(list.hasItem(item2));
    QCOMPARE(list.zIndex(item2), 1);

    auto item3 = std::make_shared<RectangleItem>();
    list.insert(item3);
    QVERIFY(list.hasItem(item3));
    QCOMPARE(list.zIndex(item3), 2);
    QCOMPARE(list.itemList().size(), 3);

    // Remove first one
    list.remove(item);
    QCOMPARE(list.itemList().size(), 2);
    QCOMPARE(list.zIndex(item3), 2);
}

void OrderedListTest::shouldClearList()
{
    OrderedList list;
    QVERIFY(list.itemList().empty());
    for (int i = 0; i < 10; i++) {
        auto item = std::make_shared<RectangleItem>();
        list.insert(item);
    }
    QCOMPARE(list.itemList().size(), 10);

    list.clear();
    QVERIFY(list.itemList().empty());
}

#include "moc_orderedlisttest.cpp"
