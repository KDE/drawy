/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "arrowutils.hpp"
#include "drawy_gui_debug.h"
using namespace Qt::Literals::StringLiterals;
QString ArrowUtils::convertArrowTypeEnumToString(ArrowUtils::ArrowType type)
{
    switch (type) {
    case ArrowUtils::ArrowType::None:
        return u"None"_s;
    case ArrowUtils::ArrowType::Arrow:
        return u"Arrow"_s;
    case ArrowUtils::ArrowType::Triangle:
        return u"Triangle"_s;
    }
    return {};
}

ArrowUtils::ArrowType ArrowUtils::convertArrowTypeStringToArrowEnum(const QString &str)
{
    if (str == u"None") {
        return ArrowUtils::ArrowType::None;
    } else if (str == u"Arrow") {
        return ArrowUtils::ArrowType::Arrow;
    } else if (str == u"Triangle") {
        return ArrowUtils::ArrowType::Triangle;
    } else {
        qCWarning(DRAWY_GUI_LOG) << "ArrowItem::ArrowType is not defined for: " << str;
    }
    return ArrowUtils::ArrowType::None;
}
