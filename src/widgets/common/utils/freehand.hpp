// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QList>
#include <QPainterPath>
#include <QPoint>

namespace Common::Utils::Freehand
{
struct StrokePoint {
    QPointF point{};
    qreal pressure{1.0};
};

/**
 * @brief: smoothness factor, the lower it is, the smoother the lines are but
 * they look more unnatural
 */
inline constexpr qreal t = 0.575;
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
 * @brief smoothens a stroke formed from a list of points
 * @param points a QList of QPointF
 * @param pressures a QList of pressure values from 0.0 to 1.0
 * @param simulatePressure whether to simulate pressure based on distance or use
 * values from `pressures`
 * @returns a QList of QPointF with the same number of points but with less
 * noise and smoother
 */
[[nodiscard]] QList<StrokePoint> getStrokePoints(const QList<QPointF> &points, const QList<qreal> &pressures, const bool simulatePressure);

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
}; // namespace Common::Utils::Freehand
