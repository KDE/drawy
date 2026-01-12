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
