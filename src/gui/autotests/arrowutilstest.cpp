/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "arrowutilstest.hpp"
#include "item/arrowutils.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(ArrowUtilsTest)
using namespace Qt::Literals::StringLiterals;
ArrowUtilsTest::ArrowUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void ArrowUtilsTest::shouldConvertArrowTypeStringToArrowEnum()
{
    QCOMPARE(ArrowUtils::convertArrowTypeStringToArrowEnum(u"Arrow"_s), ArrowUtils::ArrowType::Arrow);
    QCOMPARE(ArrowUtils::convertArrowTypeStringToArrowEnum(u"None"_s), ArrowUtils::ArrowType::None);
    QCOMPARE(ArrowUtils::convertArrowTypeStringToArrowEnum(u"Triangle"_s), ArrowUtils::ArrowType::Triangle);
    QCOMPARE(ArrowUtils::convertArrowTypeStringToArrowEnum(u"bla"_s), ArrowUtils::ArrowType::None);
    QCOMPARE(ArrowUtils::convertArrowTypeStringToArrowEnum({}), ArrowUtils::ArrowType::None);
}

void ArrowUtilsTest::shouldConvertArrowTypeEnumToString()
{
    QCOMPARE(ArrowUtils::convertArrowTypeEnumToString(ArrowUtils::ArrowType::Arrow), u"Arrow"_s);
    QCOMPARE(ArrowUtils::convertArrowTypeEnumToString(ArrowUtils::ArrowType::None), u"None"_s);
    QCOMPARE(ArrowUtils::convertArrowTypeEnumToString(ArrowUtils::ArrowType::Triangle), u"Triangle"_s);
}

#include "moc_arrowutilstest.cpp"
