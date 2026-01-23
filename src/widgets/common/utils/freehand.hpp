// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QList>
#include <QPainterPath>
#include <QPoint>
#include <QPolygonF>
#include <QtMath>

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
inline constexpr qreal t = 0.375;
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
 * @brief get length of a vector represented as a QPointF
 * @param vector the vector as a QPointF
 * @returns qreal length of the vector
 */
[[nodiscard]] qreal length(const QPointF &vector);

/**
 * @brief normalizes a vector
 * @param vector the vector you want to normalize as a QPointF
 * @returns the normalized vector as a QPointF
 */
[[nodiscard]] QPointF unitVector(const QPointF &vector);

/**
 * @brief rotates a vector by an angle theta (radians)
 * @param vector the vector you want to rotate
 * @param angle the angle by which you want to rotate it (radians)
 * @returns the rotated vector
 */
[[nodiscard]] QPointF rotateVector(const QPointF &vector, const qreal angle);

/**
 * @brief get angle between two vectors in radians
 * @param vectorA first vector
 * @param vectorB second vector
 * @returns the angle between the vectors in radians
 */
[[nodiscard]] qreal angle(const QPointF &vectorA, const QPointF &vectorB);

/**
 * @brief get dot product of two vectors
 * @param vectorA first vector
 * @param vectorB second vector
 * @returns dot product of the two vectors
 */
[[nodiscard]] qreal dotProduct(const QPointF &vectorA, const QPointF &vectorB);

/**
 * @brief rotation direction of two vectors (cross product)
 * @param vectorA first vector
 * @param vectorB second vector
 * @returns +1 if clockwise, -1 if anti-clockwise
 */
[[nodiscard]] int crossProduct(const QPointF &vectorA, const QPointF &vectorB);

/**
 * @brief interpolate a point linearly between two points
 * @param pointA first point
 * @param pointB second point
 * @param dist distance to interpolate, also known as 't'
 * @returns QPointF, interpolated point
 */
[[nodiscard]] QPointF lerp(const QPointF &pointA, const QPointF &pointB, const qreal dist);
}; // namespace Common::Utils::Freehand
