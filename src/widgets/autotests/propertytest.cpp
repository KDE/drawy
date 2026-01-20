/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "propertytest.hpp"
#include "properties/property.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(PropertyTest)
using namespace Qt::Literals::StringLiterals;

PropertyTest::PropertyTest(QObject *parent)
    : QObject{parent}
{
}

void PropertyTest::shouldConvertStringToEnum()
{
    QCOMPARE(Property::convertStringToEnum(u"BackgroundColor"_s), Property::Type::BackgroundColor);
    QCOMPARE(Property::convertStringToEnum(u"BackgroundStyle"_s), Property::Type::BackgroundStyle);
    QCOMPARE(Property::convertStringToEnum(u"StrokeWidth"_s), Property::Type::StrokeWidth);
    QCOMPARE(Property::convertStringToEnum(u"StrokeColor"_s), Property::Type::StrokeColor);
    QCOMPARE(Property::convertStringToEnum(u"Opacity"_s), Property::Type::Opacity);
    QCOMPARE(Property::convertStringToEnum(u"FontSize"_s), Property::Type::FontSize);
    QCOMPARE(Property::convertStringToEnum(u"EraserSize"_s), Property::Type::EraserSize);
    QCOMPARE(Property::convertStringToEnum(u"Actions"_s), Property::Type::Actions);
    QCOMPARE(Property::convertStringToEnum(u"ZOrder"_s), Property::Type::ZOrder);
    QCOMPARE(Property::convertStringToEnum({}), Property::Type::Null);
    QCOMPARE(Property::convertStringToEnum(u"foo"_s), Property::Type::Null);
}

void PropertyTest::shouldConvertEnumToString()
{
    QCOMPARE(Property::convertEnumToString(Property::Type::BackgroundColor), u"BackgroundColor"_s);
    QCOMPARE(Property::convertEnumToString(Property::Type::BackgroundStyle), u"BackgroundStyle"_s);
    QCOMPARE(Property::convertEnumToString(Property::Type::StrokeWidth), u"StrokeWidth"_s);
    QCOMPARE(Property::convertEnumToString(Property::Type::StrokeColor), u"StrokeColor"_s);
    QCOMPARE(Property::convertEnumToString(Property::Type::Opacity), u"Opacity"_s);
    QCOMPARE(Property::convertEnumToString(Property::Type::FontSize), u"FontSize"_s);
    QCOMPARE(Property::convertEnumToString(Property::Type::EraserSize), u"EraserSize"_s);
    QCOMPARE(Property::convertEnumToString(Property::Type::Actions), u"Actions"_s);
    QCOMPARE(Property::convertEnumToString(Property::Type::ZOrder), u"ZOrder"_s);
    QCOMPARE(Property::convertEnumToString(Property::Type::Alignment), QString());
    QCOMPARE(Property::convertEnumToString(Property::Type::ArrowStyle), QString());
    QCOMPARE(Property::convertEnumToString(Property::Type::Null), QString());
}

#include "moc_propertytest.cpp"
