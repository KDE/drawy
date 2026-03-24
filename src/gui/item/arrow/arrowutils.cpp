/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "arrowutils.hpp"
#include <KLocalizedString>
using namespace Qt::Literals::StringLiterals;

namespace ArrowUtils
{
QString toString(ArrowHead::Type arrowType)
{
    switch (arrowType) {
    case ArrowHead::Type::None:
        return u"None"_s;
    case ArrowHead::Type::Open:
        return u"Open"_s;
    case ArrowHead::Type::Unfilled:
        return u"Unfilled"_s;
    case ArrowHead::Type::Filled:
        return u"Filled"_s;
    case ArrowHead::Type::Circle:
        return u"Circle"_s;
    case ArrowHead::Type::Diamond:
        return u"Diamond"_s;
    }

    return u""_s;
}

ArrowHead::Type fromString(const QString &arrowType)
{
    if (arrowType == u"None"_s)
        return ArrowHead::Type::None;
    if (arrowType == u"Open"_s)
        return ArrowHead::Type::Open;
    if (arrowType == u"Unfilled"_s)
        return ArrowHead::Type::Unfilled;
    if (arrowType == u"Filled"_s)
        return ArrowHead::Type::Filled;
    if (arrowType == u"Circle"_s)
        return ArrowHead::Type::Circle;
    if (arrowType == u"Diamond"_s)
        return ArrowHead::Type::Diamond;

    return ArrowHead::Type::None;
}
}
