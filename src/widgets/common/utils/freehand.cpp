// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "freehand.hpp"
#include <algorithm>
#include <iterator>
#include <kstandardguiitem.h>
#include <qbrush.h>
#include <qbytearrayview.h>
#include <qdebug.h>
#include <qmath.h>

namespace Common::Utils::Freehand
{
QList<StrokePoint> getStrokePoints(const QList<QPointF> &points, const QList<qreal> &pressures, bool simulatePressure)
{
    if (points.size() != pressures.size()) {
        throw new std::logic_error("Pressures and points list have different sizes");
    }

    if (points.empty()) {
        return {};
    }

    QList<StrokePoint> result{};
    result.push_back(StrokePoint{points[0], pressures[0]});

    if (points.size() <= 1) {
        return result;
    }

    const qreal minDist{1};
    const qreal minPressure{0.5};
    const qreal distLim{16};
    for (qsizetype pos = 1; pos < points.size(); pos++) {
        const QPointF prev{result.back().point};
        QPointF interpolated{lerp(prev, points[pos], t)};

        const qreal dist{length(interpolated - prev)};
        if (dist < minDist)
            continue;

        if (simulatePressure) {
            result.push_back(StrokePoint{interpolated, std::max(minPressure, std::min(1.0, 1.0 - dist / distLim))});
        } else {
            result.push_back(StrokePoint{interpolated, pressures[pos]});
        }
    }

    result.push_back(StrokePoint{points.back(), pressures.back()});

    // smoothing out the pressures for a better result
    qsizetype windowSize{5};
    qreal pressureSum{};
    for (qsizetype pos = 0; pos < windowSize; pos++) {
        if (pos >= result.size())
            break;

        pressureSum += result[pos].pressure;
    }

    for (qsizetype pos = 0; pos < result.size(); pos++) {
        if (pos >= windowSize) {
            pressureSum += result[pos].pressure - result[pos - windowSize].pressure;
            result[pos].pressure = pressureSum / static_cast<qreal>(windowSize);
        } else {
            result[pos].pressure = pressureSum / static_cast<qreal>(std::min(result.size(), windowSize));
        }
    }

    return result;
}

QList<QPointF> getStrokePolygon(const QList<StrokePoint> &points)
{
    if (points.empty()) {
        return {};
    }

    QList<QPointF> polygonPoints{};
    const qreal dist{5};

    // if there is only one point, draw a circle
    if (points.size() == 1) {
        const QPointF radiusVector{dist, 0};
        for (qreal delta = 0, step = 1.0 / 26; delta <= 1; delta += step) {
            const QPointF point{QPointF{rotateVector(radiusVector, 2 * PI * delta) + points.back().point}};
            polygonPoints.push_back(point);
        }
        return polygonPoints;
    }

    // we do this twice so it's better to turn this into a lambda
    const auto insertCap = [dist](const StrokePoint &prev, const StrokePoint &cur, const StrokePoint &next, QList<QPointF> &polygon) -> bool {
        const QPointF vector{next.point - cur.point};
        const qreal thickness{dist * cur.pressure};

        // if it's a sharp angle, we draw a semi circular cap (looks good)
        const QPointF prevVector{cur.point - prev.point};
        const qreal curAngle{angle(vector, prevVector)};
        if (curAngle > PI / 2 && !polygon.empty()) {
            QPointF radiusVector{unitVector(QPointF{prevVector.y(), -prevVector.x()}) * thickness};

            for (qreal delta = 0, step = 1.0 / 13; delta <= 1; delta += step) {
                const QPointF point{QPointF{rotateVector(radiusVector, PI * delta) + cur.point}};
                polygon.push_back(point);
            }

            return true;
        }

        return false;
    };

    const auto insertRegularPoint =
        [dist](const StrokePoint &prev, const StrokePoint &cur, const StrokePoint &next, QList<QPointF> &polygon, const bool flip = false) -> void {
        const qreal thickness{dist * cur.pressure};
        const QPointF vector{unitVector(next.point - cur.point)};
        const QPointF prevVector{unitVector(cur.point - prev.point)};

        QPointF lerped{lerp(vector, prevVector, dotProduct(vector, prevVector))};
        QPointF perp{QPointF{lerped.y(), -lerped.x()}};
        if (flip) {
            perp *= -1;
        }

        const QPointF perpPoint{cur.point + perp * thickness};
        polygon.push_back(perpPoint);
    };

    // moving forwards
    insertRegularPoint(points[0], points[0], points[1], polygonPoints, false);
    for (qsizetype pos = 1; pos < points.size() - 1; pos++) {
        if (!insertCap(points[pos - 1], points[pos], points[pos + 1], polygonPoints)) {
            insertRegularPoint(points[pos - 1], points[pos], points[pos + 1], polygonPoints, false);
        }
    }
    insertRegularPoint(points.back(), points.back(), *std::prev(points.end(), 2), polygonPoints, true);

    // drawing the end cap
    {
        const QPointF radiusVector{polygonPoints.back() - points.back().point};
        for (qreal delta = 0, step = 1.0 / 26; delta <= 1; delta += step) {
            const QPointF point{QPointF{rotateVector(radiusVector, PI * delta) + points.back().point}};
            polygonPoints.push_back(point);
        }
    }

    // moving backwards
    insertRegularPoint(points.back(), points.back(), *std::prev(points.end(), 2), polygonPoints, false);
    for (qsizetype pos = points.size() - 2; pos >= 1; pos--) {
        if (!insertCap(points[pos + 1], points[pos], points[pos - 1], polygonPoints)) {
            insertRegularPoint(points[pos + 1], points[pos], points[pos - 1], polygonPoints, false);
        }
    }
    insertRegularPoint(points[0], points[0], points[1], polygonPoints, true);

    // drawing the start cap
    {
        const QPointF radiusVector{polygonPoints.back() - points.front().point};
        for (qreal delta = 0, step = 1.0 / 26; delta <= 1; delta += step) {
            const QPointF point{QPointF{rotateVector(radiusVector, PI * delta) + points.front().point}};
            polygonPoints.push_back(point);
        }
    }

    return polygonPoints;
}

QPainterPath getStrokePath(const QList<QPointF> &points)
{
    QPainterPath path{};
    path.moveTo(points[0]);

    for (qsizetype pos = 0; pos < points.size(); pos++) {
        QPointF curPoint{points[pos]};
        QPointF nextPoint{points[(pos + 1) % points.size()]};
        QPointF midPoint{(curPoint + nextPoint) / 2.0};

        path.quadTo(curPoint, midPoint);
    }

    path.closeSubpath();
    path.setFillRule(Qt::WindingFill);

    return path;
}

qreal length(const QPointF &point)
{
    const qreal x{point.x()};
    const qreal y{point.y()};
    return qSqrt(x * x + y * y);
}

QPointF unitVector(const QPointF &point)
{
    const qreal len{length(point)};
    if (len == 0) {
        return point;
    }

    return QPointF{point.x() / len, point.y() / len};
}

QPointF rotateVector(const QPointF &vector, const qreal angle)
{
    qreal cosTheta{qCos(angle)};
    qreal sinTheta{qSin(angle)};

    return QPointF{vector.x() * cosTheta - vector.y() * sinTheta, vector.x() * sinTheta + vector.y() * cosTheta};
}

qreal dotProduct(const QPointF &vectorA, const QPointF &vectorB)
{
    return vectorA.x() * vectorB.x() + vectorA.y() * vectorB.y();
}

qreal angle(const QPointF &vectorA, const QPointF &vectorB)
{
    const qreal lenA{length(vectorA)};
    const qreal lenB{length(vectorB)};
    const qreal dot{dotProduct(vectorA, vectorB)};

    return qAcos(dot / (lenA * lenB));
}

int crossProduct(const QPointF &vectorA, const QPointF &vectorB)
{
    return ((vectorA.x() * vectorB.y() - vectorB.x() * vectorA.y()) > 0 ? 1 : -1);
}

QPointF lerp(const QPointF &pointA, const QPointF &pointB, const qreal dist)
{
    return (1 - dist) * pointA + dist * pointB;
}
}; // namespace Common::Utils::Freehand
