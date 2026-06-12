// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "drawyglobalconfig.h"
#include <QList>
#include <QPainterPath>
#include <QPoint>

namespace Common::Utils::Freehand
{
struct StrokePoint {
    QPointF point;
    qreal pressure{1.0};
};

/**
 * @brief: smoothness factor, the lower it is, the smoother the lines are but
 * they look more unnatural
 */

inline constexpr qreal minStreamline = 0.15;
inline constexpr qreal streamlineRange = 0.85;

inline const qreal streamline = DrawyGlobalConfig::self()->streamline();
inline const qreal thinning = DrawyGlobalConfig::self()->thinning();
inline const qreal smoothing = DrawyGlobalConfig::self()->smoothing();
inline const qreal t = minStreamline + (1 - streamline) * streamlineRange;
inline constexpr qreal epsilon = 1e3; // for precision
inline constexpr qreal PI = 3.14592654;

/**
 * @brief converts a list of points into a polygon of strokes
 * @param points a QList of QPointF
 * @param pressures a QList of pressure values from 0.0 to 1.0
 * @param simulatePressure whether to simulate pressure based on distance or use
 * values from `pressures`
 * @param thickness thickness of the polygon in pixels
 * @returns QPainterPath the final polygon
 */
[[nodiscard]] QPainterPath getStroke(const QList<QPointF> &points, const QList<qreal> &pressures, const bool simulatePressure, const qreal thickness);

/**
 * @brief the same as getStroke but has an outbound parameter that outputs the pressure value of the last point
 *        it's useful for situations when you want this information to ensure seamless connection between two strokes
 */
[[nodiscard]] QPainterPath
getStroke(const QList<QPointF> &points, const QList<qreal> &pressures, const bool simulatePressure, const qreal thickness, qreal &lastPressure);

/**
 * @brief smoothens a stroke formed from a list of points
 * @param points a QList of QPointF
 * @param pressures a QList of pressure values from 0.0 to 1.0
 * @param simulatePressure whether to simulate pressure based on distance or use
 * values from `pressures`
 * @returns a QList of QPointF with the same number of points but with less
 * noise and smoother
 */
[[nodiscard]] QList<StrokePoint>
getStrokePoints(const QList<QPointF> &points, const QList<qreal> &pressures, const bool simulatePressure, const qreal thickness);

/**
 * @brief returns a polygon formed from a vector of stroke points
 * @param points a QList of StrokePoints
 * @param thickness thickness of the polygon in pixels
 * @returns a QList of QPointF
 */
[[nodiscard]] QList<QPointF> getStrokePolygon(const QList<StrokePoint> &points, const qreal thickness);

/**
 * @brief converts a stroke polygon to a smooth QPainterPath
 * @param points a QList of QPointF
 * @returns a QPainterPath
 */
[[nodiscard]] QPainterPath getStrokePath(const QList<QPointF> &points);

/**
 * @brief converts stroke points to a QPainterPath having a single stroke, no outline
 * @param points stroke points
 * @returns a QPainterPath
 */
[[nodiscard]] QPainterPath getStrokeOutline(const QList<StrokePoint> &points);

[[nodiscard]] qreal getStrokeRadius(const qreal thickness, const qreal pressure);
} // namespace Common::Utils::Freehand
