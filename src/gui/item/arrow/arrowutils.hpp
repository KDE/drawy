/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "arrowhead.hpp"
#include "libdrawygui_export.h"
#include <QString>

namespace ArrowUtils
{
[[nodiscard]] LIBDRAWYGUI_EXPORT QString toString(ArrowHead::Type arrowType);
[[nodiscard]] LIBDRAWYGUI_EXPORT ArrowHead::Type fromString(const QString &arrowType);
} // namespace ArrowUtils
