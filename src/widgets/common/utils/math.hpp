// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QLineF>
#include <QRectF>

namespace Common::Utils::Math
{
[[nodiscard]] int orientation(QPointF a, QPointF b, QPointF c);
[[nodiscard]] bool intersects(const QRectF &rectA, const QRectF &rectB);
[[nodiscard]] bool intersects(const QLineF &a, const QLineF &b);
[[nodiscard]] bool intersects(const QRectF &rect, const QLineF &line);
[[nodiscard]] bool intersects(const QRectF &rect, const QPointF &point);

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

/**
 * @brief get angle in degree from a QTransform
 */
[[nodiscard]] qreal angle(QTransform transform);
}; // namespace Common::Utils::Math
