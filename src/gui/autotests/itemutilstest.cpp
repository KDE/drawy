/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "itemutilstest.hpp"
#include "item/item.hpp"
#include "item/itemutils.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(ItemUtilsTest)
using namespace Qt::Literals::StringLiterals;
ItemUtilsTest::ItemUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void ItemUtilsTest::shouldConvertItemTypeStringToEnum()
{
    QCOMPARE(ItemUtils::convertItemTypeStringToEnum(u"FREEFORM"_s), Item::FormType::Freeform);
    QCOMPARE(ItemUtils::convertItemTypeStringToEnum(u"RECTANGLE"_s), Item::FormType::Rectangle);
    QCOMPARE(ItemUtils::convertItemTypeStringToEnum(u"ELLIPSE"_s), Item::FormType::Ellipse);
    QCOMPARE(ItemUtils::convertItemTypeStringToEnum(u"LINE"_s), Item::FormType::Line);
    QCOMPARE(ItemUtils::convertItemTypeStringToEnum(u"ARROW"_s), Item::FormType::Arrow);
    QCOMPARE(ItemUtils::convertItemTypeStringToEnum(u"TEXT"_s), Item::FormType::Text);
    QCOMPARE(ItemUtils::convertItemTypeStringToEnum(u"GROUP"_s), Item::FormType::Group);
    QCOMPARE(ItemUtils::convertItemTypeStringToEnum(u"DIAMOND"_s), Item::FormType::Diamond);
    QCOMPARE(ItemUtils::convertItemTypeStringToEnum(u"IMAGE"_s), Item::FormType::Image);
    QCOMPARE(ItemUtils::convertItemTypeStringToEnum(u"CUSTOM"_s), Item::FormType::Custom);
    QCOMPARE(ItemUtils::convertItemTypeStringToEnum({}), Item::FormType::Invalid);
    QCOMPARE(ItemUtils::convertItemTypeStringToEnum(u"foo"_s), Item::FormType::Invalid);
}

void ItemUtilsTest::shouldConvertEnumItemTypeToString()
{
    QCOMPARE(ItemUtils::convertItemTypeEnumToString(Item::FormType::Freeform), u"FREEFORM"_s);
    QCOMPARE(ItemUtils::convertItemTypeEnumToString(Item::FormType::Custom), u"CUSTOM"_s);
    QCOMPARE(ItemUtils::convertItemTypeEnumToString(Item::FormType::Rectangle), u"RECTANGLE"_s);
    QCOMPARE(ItemUtils::convertItemTypeEnumToString(Item::FormType::Ellipse), u"ELLIPSE"_s);
    QCOMPARE(ItemUtils::convertItemTypeEnumToString(Item::FormType::Line), u"LINE"_s);
    QCOMPARE(ItemUtils::convertItemTypeEnumToString(Item::FormType::Arrow), u"ARROW"_s);
    QCOMPARE(ItemUtils::convertItemTypeEnumToString(Item::FormType::Text), u"TEXT"_s);
    QCOMPARE(ItemUtils::convertItemTypeEnumToString(Item::FormType::Group), u"GROUP"_s);
    QCOMPARE(ItemUtils::convertItemTypeEnumToString(Item::FormType::Diamond), u"DIAMOND"_s);
    QCOMPARE(ItemUtils::convertItemTypeEnumToString(Item::FormType::Image), u"IMAGE"_s);
    QCOMPARE(ItemUtils::convertItemTypeEnumToString(Item::FormType::Invalid), QString());
}

void ItemUtilsTest::shouldConvertEnumStrokeTypeToString()
{
    QCOMPARE(ItemUtils::convertItemStrokeTypeEnumToString(Item::StrokeType::Solid), u"Solid"_s);
    QCOMPARE(ItemUtils::convertItemStrokeTypeEnumToString(Item::StrokeType::DashLine), u"DashLine"_s);
    QCOMPARE(ItemUtils::convertItemStrokeTypeEnumToString(Item::StrokeType::DotLine), u"DotLine"_s);
    QCOMPARE(ItemUtils::convertItemStrokeTypeEnumToString(Item::StrokeType::Invalid), QString{});
}

void ItemUtilsTest::shouldConvertStrokeTypeStringToEnum()
{
    QCOMPARE(ItemUtils::convertStrokeTypeStringToEnum(u"Solid"_s), Item::StrokeType::Solid);
    QCOMPARE(ItemUtils::convertStrokeTypeStringToEnum(u"DashLine"_s), Item::StrokeType::DashLine);
    QCOMPARE(ItemUtils::convertStrokeTypeStringToEnum(u"DotLine"_s), Item::StrokeType::DotLine);
    QCOMPARE(ItemUtils::convertStrokeTypeStringToEnum(u"Solidsdfsdf"_s), Item::StrokeType::Invalid);
    QCOMPARE(ItemUtils::convertStrokeTypeStringToEnum(QString()), Item::StrokeType::Invalid);
}

void ItemUtilsTest::shouldConvertItemStrokeTypeStringToPenStyle()
{
    QCOMPARE(ItemUtils::convertItemStrokeTypeStringToPenStyle(u"Solid"_s), Qt::PenStyle::SolidLine);
    QCOMPARE(ItemUtils::convertItemStrokeTypeStringToPenStyle(u"DashLine"_s), Qt::PenStyle::DashLine);
    QCOMPARE(ItemUtils::convertItemStrokeTypeStringToPenStyle(u"DotLine"_s), Qt::PenStyle::DotLine);
    QCOMPARE(ItemUtils::convertItemStrokeTypeStringToPenStyle(u"Solidsdfsdf"_s), Qt::PenStyle::SolidLine);
    QCOMPARE(ItemUtils::convertItemStrokeTypeStringToPenStyle(QString()), Qt::PenStyle::SolidLine);
}

void ItemUtilsTest::shouldConvertEnumBackgroundTypeToString()
{
    QCOMPARE(ItemUtils::convertItemBackgroundTypeEnumToString(Item::BackgroundType::Solid), u"Solid"_s);
    QCOMPARE(ItemUtils::convertItemBackgroundTypeEnumToString(Item::BackgroundType::None), u"None"_s);
    QCOMPARE(ItemUtils::convertItemBackgroundTypeEnumToString(Item::BackgroundType::Diagonal), u"Diagonal"_s);
    QCOMPARE(ItemUtils::convertItemBackgroundTypeEnumToString(Item::BackgroundType::CrossDiagonal), u"CrossDiagonal"_s);
    QCOMPARE(ItemUtils::convertItemBackgroundTypeEnumToString(Item::BackgroundType::Invalid), QString{});
}

void ItemUtilsTest::shouldConvertBackgroundTypeStringToEnum()
{
    QCOMPARE(ItemUtils::convertBackgroundTypeStringToEnum(u"Solid"_s), Item::BackgroundType::Solid);
    QCOMPARE(ItemUtils::convertBackgroundTypeStringToEnum(u"Diagonal"_s), Item::BackgroundType::Diagonal);
    QCOMPARE(ItemUtils::convertBackgroundTypeStringToEnum(u"CrossDiagonal"_s), Item::BackgroundType::CrossDiagonal);
    QCOMPARE(ItemUtils::convertBackgroundTypeStringToEnum(u"Solidsdfsdf"_s), Item::BackgroundType::Invalid);
    QCOMPARE(ItemUtils::convertBackgroundTypeStringToEnum(u"None"_s), Item::BackgroundType::None);
    QCOMPARE(ItemUtils::convertBackgroundTypeStringToEnum(QString()), Item::BackgroundType::Invalid);
}

void ItemUtilsTest::shouldConvertItemBackgroundTypeStringToBrushStyle()
{
    QCOMPARE(ItemUtils::convertItemBackgroundTypeStringToBrushStyle(u"Solid"_s), Qt::BrushStyle::SolidPattern);
    QCOMPARE(ItemUtils::convertItemBackgroundTypeStringToBrushStyle(u"Diagonal"_s), Qt::BrushStyle::BDiagPattern);
    QCOMPARE(ItemUtils::convertItemBackgroundTypeStringToBrushStyle(u"CrossDiagonal"_s), Qt::BrushStyle::DiagCrossPattern);
    QCOMPARE(ItemUtils::convertItemBackgroundTypeStringToBrushStyle(u"Solidsdfsdf"_s), Qt::BrushStyle::SolidPattern);
    QCOMPARE(ItemUtils::convertItemBackgroundTypeStringToBrushStyle(QString()), Qt::BrushStyle::SolidPattern);
}

#include "moc_itemutilstest.cpp"
