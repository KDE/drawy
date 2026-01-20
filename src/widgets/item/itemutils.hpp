/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include "item/item.hpp"
#include "libdrawywidgets_private_export.h"
namespace ItemUtils
{
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT Item::Type convertItemTypeStringToEnum(const QString &str);
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT QString convertItemTypeEnumToString(Item::Type type);
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT Item::StrokeType convertStrokeTypeStringToEnum(const QString &str);
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT QString convertItemStrokeTypeEnumToString(Item::StrokeType type);
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT Qt::PenStyle convertItemStrokeTypeStringToPenStyle(const QString &penStyle);
}; // namespace ItemUtils
