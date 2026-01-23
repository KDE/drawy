/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "libdrawywidgets_private_export.h"
#include <QString>

namespace AutoSaveJobUtil
{
LIBDRAWYWIDGETS_TESTS_EXPORT void createAutoSaveStandardPath();
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT QString temporaryFileName();
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT bool checkExistingAutoSaveFile();
LIBDRAWYWIDGETS_TESTS_EXPORT void removeAutoSaveFile();
}; // namespace AutoSaveJobUtil
