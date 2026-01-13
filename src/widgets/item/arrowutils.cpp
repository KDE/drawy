/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "arrowutils.hpp"
#include "drawy_debug.h"
using namespace Qt::Literals::StringLiterals;
QString ArrowUtils::convertArrowTypeEnumToString(ArrowItem::ArrowType type)
{
    switch (type) {
    case ArrowItem::ArrowType::None:
        return u"None"_s;
    case ArrowItem::ArrowType::Arrow:
        return u"Arrow"_s;
    case ArrowItem::ArrowType::Triangle:
        return u"Triangle"_s;
    }
    return {};
}

ArrowItem::ArrowType ArrowUtils::convertArrowTypeStringToArrowEnum(const QString &str)
{
    if (str == u"None") {
        return ArrowItem::ArrowType::None;
    } else if (str == u"Arrow") {
        return ArrowItem::ArrowType::Arrow;
    } else if (str == u"Triangle") {
        return ArrowItem::ArrowType::Triangle;
    } else {
        qCWarning(DRAWY_LOG) << "ArrowItem::ArrowType is not defined for: " << str;
    }
    return ArrowItem::ArrowType::None;
}
