/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "libdrawywidgets_private_export.h"
#include <QString>
namespace ArrowUtils
{
enum class ArrowType : int8_t {
    None,
    Arrow,
    Triangle,
    // TODO add more in the future
};
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT QString convertArrowTypeEnumToString(ArrowUtils::ArrowType type);
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT ArrowUtils::ArrowType convertArrowTypeStringToArrowEnum(const QString &penStyle);
}; // namespace ArrowUtils
