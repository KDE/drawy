/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "itemutils.hpp"
#include "drawy_debug.h"
#include "item/arrow.hpp"
#include "item/diamond.hpp"
#include "item/ellipse.hpp"
#include "item/freeform.hpp"
#include "item/group.hpp"
#include "item/line.hpp"
#include "item/rectangle.hpp"
#include "item/text.hpp"

using namespace Qt::Literals::StringLiterals;
Item::FormType ItemUtils::convertItemTypeStringToEnum(const QString &str)
{
    if (str == u"FREEFORM") {
        return Item::FormType::Freeform;
    } else if (str == u"RECTANGLE") {
        return Item::FormType::Rectangle;
    } else if (str == u"ELLIPSE") {
        return Item::FormType::Ellipse;
    } else if (str == u"LINE") {
        return Item::FormType::Line;
    } else if (str == u"ARROW") {
        return Item::FormType::Arrow;
    } else if (str == u"TEXT") {
        return Item::FormType::Text;
    } else if (str == u"GROUP") {
        return Item::FormType::Group;
    } else if (str == u"DIAMOND") {
        return Item::FormType::Diamond;
    } else if (str == u"CUSTOM") {
        return Item::FormType::Custom;
    } else {
        qCWarning(DRAWY_LOG) << "Item::Type is not defined for: " << str;
    }
    return Item::FormType::Invalid;
}

QString ItemUtils::convertItemTypeEnumToString(Item::FormType type)
{
    switch (type) {
    case Item::FormType::Freeform:
        return u"FREEFORM"_s;
    case Item::FormType::Rectangle:
        return u"RECTANGLE"_s;
    case Item::FormType::Ellipse:
        return u"ELLIPSE"_s;
    case Item::FormType::Line:
        return u"LINE"_s;
    case Item::FormType::Arrow:
        return u"ARROW"_s;
    case Item::FormType::Text:
        return u"TEXT"_s;
    case Item::FormType::Group:
        return u"GROUP"_s;
    case Item::FormType::Diamond:
        return u"DIAMOND"_s;
    case Item::FormType::Custom:
        return u"CUSTOM"_s;
    case Item::FormType::Invalid:
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

std::shared_ptr<Item> ItemUtils::createItemFromType(Item::FormType type)
{
    std::shared_ptr<Item> item;
    switch (type) {
    case Item::FormType::Freeform: {
        item = std::make_shared<FreeformItem>();
        break;
    }
    case Item::FormType::Rectangle: {
        item = std::make_shared<RectangleItem>();
        break;
    }
    case Item::FormType::Line: {
        item = std::make_shared<LineItem>();
        break;
    }
    case Item::FormType::Arrow: {
        item = std::make_shared<ArrowItem>();
        break;
    }
    case Item::FormType::Ellipse: {
        item = std::make_shared<EllipseItem>();
        break;
    }
    case Item::FormType::Text: {
        item = std::make_shared<TextItem>();
        break;
    }
    case Item::FormType::Group: {
        item = std::make_shared<GroupItem>();
        break;
    }
    case Item::FormType::Diamond: {
        item = std::make_shared<DiamondItem>();
        break;
    }
    case Item::FormType::Custom: {
        break;
    }
    case Item::FormType::Invalid:
        qCWarning(DRAWY_LOG) << "Item::FormType::Invalid it's a bug";
        break;
    }
    return item;
}
