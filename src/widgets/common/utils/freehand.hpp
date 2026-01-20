#pragma once

#include <QList>
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
inline constexpr qreal t = 0.575;
inline constexpr qreal epsilon = 1e3; // for precision
inline constexpr qreal PI = 3.14592654;

/**
 * @brief smoothens a stroke formed from a list of points
 * @param points a QList of QPointF
 * @returns a QList of QPointF with the same number of points but with less
 * noise and smoother
 */
QList<StrokePoint> getStrokePoints(const QList<QPointF> &points, const QList<qreal> &pressures);

/**
 * @brief returns a polygon formed from a vector of stroke points
 * @param points a QList of StrokePoints
 * @returns a QList of QPointF
 */
QList<QPointF> getStrokePolygon(const QList<StrokePoint> &points);

/**
 * @brief get length of a vector represented as a QPointF
 * @param vector the vector as a QPointF
 * @returns qreal length of the vector
 */
qreal length(const QPointF &vector);

/**
 * @brief normalizes a vector
 * @param vector the vector you want to normalize as a QPointF
 * @returns the normalized vector as a QPointF
 */
QPointF unitVector(const QPointF &vector);

/**
 * @brief rotates a vector by an angle theta (radians)
 * @param vector the vector you want to rotate
 * @param angle the angle by which you want to rotate it (radians)
 * @returns the rotated vector
 */
QPointF rotateVector(const QPointF &vector, const qreal angle);

/**
 * @brief get angle between two vectors in radians
 * @param vectorA first vector
 * @param vectorB second vector
 * @returns the angle between the vectors in radians
 */
qreal angle(const QPointF &vectorA, const QPointF &vectorB);

/**
 * @brief get dot product of two vectors
 * @param vectorA first vector
 * @param vectorB second vector
 * @returns dot product of the two vectors
 */
qreal dotProduct(const QPointF &vectorA, const QPointF &vectorB);

/**
 * @brief rotation direction of two vectors (cross product)
 * @param vectorA first vector
 * @param vectorB second vector
 * @returns +1 if clockwise, -1 if anti-clockwise
 */
int crossProduct(const QPointF &vectorA, const QPointF &vectorB);

/**
 * @brief interpolate a point linearly between two points
 * @param pointA first point
 * @param pointB second point
 * @param dist distance to interpolate, also known as 't'
 * @returns QPointF, interpolated point
 */
QPointF lerp(const QPointF &pointA, const QPointF &pointB, const qreal dist);
}; // namespace Common::Utils::Freehand
