/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QColor>
#include <QList>

namespace PropertyUtils
{
[[nodiscard]] QList<QColor> strokeDefaultColors();
[[nodiscard]] QList<QColor> backgroundDefaultColors();
};
