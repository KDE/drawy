/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "libdrawygui_export.h"
namespace TransformHandlerUtils
{
enum class LIBDRAWYGUI_EXPORT Type {
    MoveTransformHandler,
    ResizeTransformHandler,
    RotateTransformHandler,
    LineMoveTransformHandler,
    LineResizeTransformHandler,
};
}
