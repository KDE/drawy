/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include "item/item.hpp"
#include "libdrawygui_export.h"
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

[[nodiscard]] LIBDRAWYGUI_EXPORT Item::FormType convertItemTypeStringToEnum(const QString &str);
[[nodiscard]] LIBDRAWYGUI_EXPORT QString convertItemTypeEnumToString(Item::FormType type);
[[nodiscard]] LIBDRAWYGUI_EXPORT Item::StrokeType convertStrokeTypeStringToEnum(const QString &str);
[[nodiscard]] LIBDRAWYGUI_EXPORT QString convertItemStrokeTypeEnumToString(Item::StrokeType type);
[[nodiscard]] LIBDRAWYGUI_EXPORT Qt::PenStyle convertItemStrokeTypeStringToPenStyle(const QString &penStyle);

[[nodiscard]] LIBDRAWYGUI_EXPORT QString convertItemBackgroundTypeEnumToString(Item::BackgroundType type);
[[nodiscard]] LIBDRAWYGUI_EXPORT Qt::BrushStyle convertItemBackgroundTypeStringToBrushStyle(const QString &brushStyle);
[[nodiscard]] LIBDRAWYGUI_EXPORT Item::BackgroundType convertBackgroundTypeStringToEnum(const QString &str);

[[nodiscard]] LIBDRAWYGUI_EXPORT QString convertFontStyleToString(int style);
[[nodiscard]] LIBDRAWYGUI_EXPORT int convertStringToFontStyle(const QString &str);

[[nodiscard]] LIBDRAWYGUI_EXPORT QString convertTextAlignmentToString(int alignment);
[[nodiscard]] LIBDRAWYGUI_EXPORT int convertStringToTextAlignment(const QString &str);

[[nodiscard]] LIBDRAWYGUI_EXPORT QString convertItemCornerRectangleTypeEnumToString(Item::CornerRectangleType type);
[[nodiscard]] LIBDRAWYGUI_EXPORT Item::CornerRectangleType convertItemCornerRectangleTypeStringToEnum(const QString &str);

[[nodiscard]] LIBDRAWYGUI_EXPORT QString convertListStyleToString(int style);
[[nodiscard]] LIBDRAWYGUI_EXPORT int convertStringToListStyle(const QString &str);

[[nodiscard]] std::shared_ptr<Item> createItemFromType(Item::FormType type);

} // namespace ItemUtils
