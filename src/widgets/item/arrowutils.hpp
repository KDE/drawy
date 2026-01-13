/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "item/arrow.hpp"
#include "libdrawywidgets_private_export.h"

namespace ArrowUtils
{
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT QString convertArrowTypeEnumToString(ArrowItem::ArrowType type);
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT ArrowItem::ArrowType convertArrowTypeStringToArrowEnum(const QString &penStyle);
};
