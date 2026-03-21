/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "arrowhead.hpp"
#include <QString>

namespace ArrowUtils
{
QString toString(ArrowHead::Type arrowType);
ArrowHead::Type fromString(const QString &arrowType);
} // namespace ArrowUtils
