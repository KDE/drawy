// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QColor>
#include <QMargins>
#include <QSize>
#include <array>

namespace Common
{
inline constexpr QMargins cleanupMargin{10, 10, 10, 10}; // in pixels
inline constexpr int defaultEraserSize{30};

inline constexpr QColor eraserBorderColor{255, 0, 0};
inline constexpr QColor eraserBackgroundColor{255, 0, 0, 50};
inline constexpr int eraserBorderWidth{2};

inline constexpr QColor selectionBorderColor{67, 135, 244, 255};
inline constexpr QColor selectionBackgroundColor{67, 135, 244, 50};

inline constexpr unsigned int erasedItemColor{0x6E6E6E96};

inline constexpr QColor lightBackgroundColor{248, 249, 250};
inline constexpr QColor darkBackgroundColor{18, 18, 18};

inline constexpr QColor lightForegroundColor{30, 30, 30};
inline constexpr QColor darkForegroundColor{211, 211, 211};

inline constexpr int maxItemOpacity{255};
inline constexpr int eraseItemOpacity{50};

inline constexpr int boundingBoxPadding{10}; // in pixels

inline constexpr int translationDelta{1}; // in pixels
inline constexpr int shiftTranslationDelta{10}; // in pixels, when holding shift

inline constexpr double defaultTextBoxWidth{5}; // in pixels
inline constexpr std::array<QChar, 38> wordSeparators{'\t', '\n', '\v', '\f', '\r', ' ', // whitespace
                                                      '!',  '"',  '#',  '$',  '%',  '&', '\'', '(',  ')', '*', '+', ',', '-', '.', '/', ':',
                                                      ';',  '<',  '=',  '>',  '?',  '@', '[',  '\\', ']', '^', '_', '`', '{', '|', '}', '~'};

inline constexpr int doubleClickInterval{300}; // milliseconds

inline constexpr qreal tabStopDistance{4};

inline constexpr std::string_view drawyFileExt{"drawy"};
}; // namespace Common
