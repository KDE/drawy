/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "itemtest.h"
#include "item/item.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(ItemTest)
using namespace Qt::Literals::StringLiterals;
ItemTest::ItemTest(QObject *parent)
    : QObject{parent}
{
}

void ItemTest::shouldConvertStringToEnum()
{
    QCOMPARE(Item::convertStringToEnum(u"FREEFORM"_s), Item::Type::Freeform);
    QCOMPARE(Item::convertStringToEnum(u"RECTANGLE"_s), Item::Type::Rectangle);
    QCOMPARE(Item::convertStringToEnum(u"ELLIPSE"_s), Item::Type::Ellipse);
    QCOMPARE(Item::convertStringToEnum(u"LINE"_s), Item::Type::Line);
    QCOMPARE(Item::convertStringToEnum(u"ARROW"_s), Item::Type::Arrow);
    QCOMPARE(Item::convertStringToEnum(u"TEXT"_s), Item::Type::Text);
    QCOMPARE(Item::convertStringToEnum(u"GROUP"_s), Item::Type::Group);
    QCOMPARE(Item::convertStringToEnum({}), Item::Type::Invalid);
    QCOMPARE(Item::convertStringToEnum(u"foo"_s), Item::Type::Invalid);
}

void ItemTest::shouldConvertEnumToString()
{
    QCOMPARE(Item::convertEnumToString(Item::Type::Freeform), u"FREEFORM"_s);
    QCOMPARE(Item::convertEnumToString(Item::Type::Rectangle), u"RECTANGLE"_s);
    QCOMPARE(Item::convertEnumToString(Item::Type::Ellipse), u"ELLIPSE"_s);
    QCOMPARE(Item::convertEnumToString(Item::Type::Line), u"LINE"_s);
    QCOMPARE(Item::convertEnumToString(Item::Type::Arrow), u"ARROW"_s);
    QCOMPARE(Item::convertEnumToString(Item::Type::Text), u"TEXT"_s);
    QCOMPARE(Item::convertEnumToString(Item::Type::Group), u"GROUP"_s);
    QCOMPARE(Item::convertEnumToString(Item::Type::Invalid), QString());
}
