// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <KLocalizedString>

#include <QColor>
#include <QMargins>
#include <QPen>
#include <QSize>
#include <array>
using namespace Qt::Literals::StringLiterals;

namespace Common
{
inline constexpr QSize textAlignmentIconSize{25, 25};

inline constexpr QMargins cleanupMargin{10, 10, 10, 10}; // in pixels

inline constexpr QColor eraserBorderColor{255, 0, 0};
inline constexpr QColor eraserBackgroundColor{255, 0, 0, 50};
inline constexpr int eraserBorderWidth{2};

inline constexpr int selectionBorderWidth{2};
inline constexpr QColor selectionBorderColor{67, 135, 244, 255};
inline constexpr QColor selectionBackgroundColor{67, 135, 244, 50};
inline constexpr qreal selectionHandleSize{10.0};

inline constexpr QSize fontStyleIconSize{25, 25};
inline constexpr int minFontSize{1};

inline QPen selectionBorderPen()
{
    return QPen{selectionBorderColor, selectionBorderWidth};
}

inline constexpr unsigned int erasedItemColor{0x6E6E6E96};

inline constexpr QColor darkBackgroundColor{18, 18, 18};

inline constexpr int maxItemOpacity{255};
inline constexpr int erasedItemOpacityDecrease{30}; // % to reduce opacity by when erasing items

inline constexpr int doubleClickInterval{300}; // milliseconds

inline constexpr qreal tabStopDistance{4};

inline constexpr qreal zoomOutLimit{0.1}; // 10%
inline constexpr qreal zoomInLimit{30}; // 100% * 30 = 3000% zoom
inline constexpr qreal zoomMultiplier{1.1}; // it's standard practice to multiply/divide by 1.1 when zooming
inline constexpr int zoomRenderWaitTime{250}; // milliseconds

inline constexpr QSize maxItemCacheCellSize{500, 500}; // in pixels
inline constexpr int itemCacheMultiplier{2}; // For per-item cache; increasing this value decreases thrashing but
                                             // increases memory usage
inline constexpr int viewportCacheMultiplier{2}; // For viewport cache; increasing this value decreases thrashing but
                                                 // increases memory usage

inline constexpr int pastedImageBoxSize{250}; // the size of the box that a dropped image will be scaled to fit in

inline const QString drawyFileExt{u"drawy"_s};

inline const QString drawyClipboardMime{u"application/x-drawy-clipboard"_s};

inline constexpr int translationDelta{1}; // in px
inline constexpr int shiftTranslationDelta{10}; // in px
inline constexpr int selectionCursorHitSize{10}; // in px, the size for detecting collisions with the cursor

inline const QString configSession{u"Session"_s};
inline const QString configSessionLastSavedFile{u"LastSavedFile"_s};
inline const QString configSessionLastSavedFileModified{u"LastSavedFileModified"_s};
} // namespace Common
