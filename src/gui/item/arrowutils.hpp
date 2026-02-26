/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "libdrawygui_export.h"
#include <QString>
namespace ArrowUtils
{
enum class ArrowType : int8_t {
    None,
    Arrow,
    Triangle,
    FullTriangle,
    Circle,
    FullCircle,
    Line,
    // TODO add more in the future
};
[[nodiscard]] LIBDRAWYGUI_EXPORT QString convertArrowTypeEnumToString(ArrowUtils::ArrowType type);
[[nodiscard]] LIBDRAWYGUI_EXPORT ArrowUtils::ArrowType convertArrowTypeStringToArrowEnum(const QString &penStyle);
} // namespace ArrowUtils
