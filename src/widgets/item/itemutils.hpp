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
enum class AlignType : uint8_t {
    Unknown = 0,
    AlignLeft,
    AlignHorizontalCenter,
    AlignRight,
    AlignTop,
    AlignVerticalCenter,
    AlignBottom,
};

enum class ZorderMove : uint8_t {
    BringForward = 0,
    SendBackward,
    SendToBack,
    BringToFront,
};

[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT Item::Type convertItemTypeStringToEnum(const QString &str);
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT QString convertItemTypeEnumToString(Item::Type type);
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT Item::StrokeType convertStrokeTypeStringToEnum(const QString &str);
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT QString convertItemStrokeTypeEnumToString(Item::StrokeType type);
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT Qt::PenStyle convertItemStrokeTypeStringToPenStyle(const QString &penStyle);

[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT QString convertItemBackgroundTypeEnumToString(Item::BackgroundType type);
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT Qt::BrushStyle convertItemBackgroundTypeStringToBrushStyle(const QString &brushStyle);
[[nodiscard]] LIBDRAWYWIDGETS_TESTS_EXPORT Item::BackgroundType convertBackgroundTypeStringToEnum(const QString &str);

[[nodiscard]] std::shared_ptr<Item> createItemFromType(Item::Type type);

}; // namespace ItemUtils
