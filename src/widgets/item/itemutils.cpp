/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "itemutils.hpp"
#include "drawy_debug.h"

using namespace Qt::Literals::StringLiterals;
Item::Type ItemUtils::convertItemTypeStringToEnum(const QString &str)
{
    if (str == u"FREEFORM") {
        return Item::Type::Freeform;
    } else if (str == u"RECTANGLE") {
        return Item::Type::Rectangle;
    } else if (str == u"ELLIPSE") {
        return Item::Type::Ellipse;
    } else if (str == u"LINE") {
        return Item::Type::Line;
    } else if (str == u"ARROW") {
        return Item::Type::Arrow;
    } else if (str == u"TEXT") {
        return Item::Type::Text;
    } else if (str == u"GROUP") {
        return Item::Type::Group;
    } else if (str == u"DIAMOND") {
        return Item::Type::Diamond;
    } else {
        qCWarning(DRAWY_LOG) << "Item::Type is not defined for: " << str;
    }
    return Item::Type::Invalid;
}

QString ItemUtils::convertItemTypeEnumToString(Item::Type type)
{
    switch (type) {
    case Item::Type::Freeform:
        return u"FREEFORM"_s;
    case Item::Type::Rectangle:
        return u"RECTANGLE"_s;
    case Item::Type::Ellipse:
        return u"ELLIPSE"_s;
    case Item::Type::Line:
        return u"LINE"_s;
    case Item::Type::Arrow:
        return u"ARROW"_s;
    case Item::Type::Text:
        return u"TEXT"_s;
    case Item::Type::Group:
        return u"GROUP"_s;
    case Item::Type::Diamond:
        return u"DIAMOND"_s;
    case Item::Type::Invalid:
        qCWarning(DRAWY_LOG) << "Don't invalid type";
    }
    return {};
}

Item::StrokeType ItemUtils::convertStrokeTypeStringToEnum(const QString &str)
{
    if (str == u"Solid") {
        return Item::StrokeType::Solid;
    } else if (str == u"DashLine") {
        return Item::StrokeType::DashLine;
    } else if (str == u"DotLine") {
        return Item::StrokeType::DotLine;
    } else {
        qCWarning(DRAWY_LOG) << "Item::StrokeType is not defined for: " << str;
    }
    return Item::StrokeType::Invalid;
}

QString ItemUtils::convertItemStrokeTypeEnumToString(Item::StrokeType type)
{
    switch (type) {
    case Item::StrokeType::Solid:
        return u"Solid"_s;
    case Item::StrokeType::DashLine:
        return u"DashLine"_s;
    case Item::StrokeType::DotLine:
        return u"DotLine"_s;
    case Item::StrokeType::Invalid:
        qCWarning(DRAWY_LOG) << "Don't invalid StrokeType";
        break;
    }
    return {};
}

Qt::PenStyle ItemUtils::convertItemStrokeTypeStringToPenStyle(const QString &penStyle)
{
    if (penStyle == u"Solid") {
        return Qt::PenStyle::SolidLine;
    } else if (penStyle == u"DashLine") {
        return Qt::PenStyle::DashLine;
    } else if (penStyle == u"DotLine") {
        return Qt::PenStyle::DotLine;
    } else {
        qCWarning(DRAWY_LOG) << "Item::StrokeType is not defined for: " << penStyle;
    }
    return Qt::PenStyle::SolidLine;
}

Item::BackgroundType ItemUtils::convertBackgroundTypeStringToEnum(const QString &str)
{
    if (str == u"Solid") {
        return Item::BackgroundType::Solid;
    } else if (str == u"Diagonal") {
        return Item::BackgroundType::Diagonal;
    } else if (str == u"CrossDiagonal") {
        return Item::BackgroundType::CrossDiagonal;
    } else {
        qCWarning(DRAWY_LOG) << "Item::BackgroundType is not defined for: " << str;
    }
    return Item::BackgroundType::Invalid;
}

QString ItemUtils::convertItemBackgroundTypeEnumToString(Item::BackgroundType type)
{
    switch (type) {
    case Item::BackgroundType::Solid:
        return u"Solid"_s;
    case Item::BackgroundType::Diagonal:
        return u"Diagonal"_s;
    case Item::BackgroundType::CrossDiagonal:
        return u"CrossDiagonal"_s;
    case Item::BackgroundType::Invalid:
        qCWarning(DRAWY_LOG) << "Don't invalid BackgroundType";
        break;
    }
    return {};
}

Qt::BrushStyle ItemUtils::convertItemBackgroundTypeStringToBrushStyle(const QString &brushStyle)
{
    if (brushStyle == u"Solid") {
        return Qt::BrushStyle::SolidPattern;
    } else if (brushStyle == u"Diagonal") {
        return Qt::BrushStyle::BDiagPattern;
    } else if (brushStyle == u"CrossDiagonal") {
        return Qt::BrushStyle::DiagCrossPattern;
    } else {
        qCWarning(DRAWY_LOG) << "Item::BackgroundType is not defined for: " << brushStyle;
    }
    return Qt::BrushStyle::SolidPattern;
}
