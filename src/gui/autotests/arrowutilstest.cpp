/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "arrowutilstest.hpp"
#include "item/arrow/arrowhead.hpp"
#include "item/arrow/arrowutils.hpp"
#include <QTest>
using namespace Qt::Literals::StringLiterals;

QTEST_GUILESS_MAIN(ArrowUtilsTest)
ArrowUtilsTest::ArrowUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void ArrowUtilsTest::shouldConvertArrowTypeStringToArrowEnum()
{
    QCOMPARE(ArrowUtils::fromString(u"None"_s), ArrowHead::Type::None);
    QCOMPARE(ArrowUtils::fromString(u"Open"_s), ArrowHead::Type::Open);
    QCOMPARE(ArrowUtils::fromString(u"Unfilled"_s), ArrowHead::Type::Unfilled);
    QCOMPARE(ArrowUtils::fromString(u"Filled"_s), ArrowHead::Type::Filled);
    QCOMPARE(ArrowUtils::fromString(u"Circle"_s), ArrowHead::Type::Circle);
    QCOMPARE(ArrowUtils::fromString(u"Diamond"_s), ArrowHead::Type::Diamond);
}

void ArrowUtilsTest::shouldConvertArrowTypeEnumToString()
{
    QCOMPARE(ArrowUtils::toString(ArrowHead::Type::None), u"None"_s);
    QCOMPARE(ArrowUtils::toString(ArrowHead::Type::Open), u"Open"_s);
    QCOMPARE(ArrowUtils::toString(ArrowHead::Type::Unfilled), u"Unfilled"_s);
    QCOMPARE(ArrowUtils::toString(ArrowHead::Type::Filled), u"Filled"_s);
    QCOMPARE(ArrowUtils::toString(ArrowHead::Type::Circle), u"Circle"_s);
    QCOMPARE(ArrowUtils::toString(ArrowHead::Type::Diamond), u"Diamond"_s);
}

#include "moc_arrowutilstest.cpp"
