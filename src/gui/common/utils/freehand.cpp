// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "freehand.hpp"
#include "math.hpp"
#include <QtMath>

namespace Common::Utils::Freehand
{
QPainterPath getStroke(const QList<QPointF> &points, const QList<qreal> &pressures, const bool simulatePressure, const qreal thickness)
{
    return getStrokePath(getStrokePolygon(getStrokePoints(points, pressures, simulatePressure), thickness));
}

QList<StrokePoint> getStrokePoints(const QList<QPointF> &points, const QList<qreal> &pressures, const bool simulatePressure)
{
    using namespace Common::Utils::Math;
    if (points.size() != pressures.size()) {
        throw std::logic_error("Pressures and points list have different sizes");
    }

    if (points.empty()) {
        return QList<StrokePoint>();
    }

    QList<StrokePoint> result{};
    result.push_back(StrokePoint{points[0], pressures[0]});

    if (points.size() == 1) {
        return result;
    }

    const qreal minDist{1};
    const qreal minPressure{0.5};
    const qreal distLim{16};
    for (qsizetype pos = 1; pos < points.size(); pos++) {
        const QPointF prev{result.back().point};
        const QPointF interpolated{lerp(prev, points[pos], t)};

        const qreal dist{length(interpolated - prev)};

        if (dist < minDist) {
            continue;
        }

        if (simulatePressure) {
            result.push_back(StrokePoint{interpolated, std::max(minPressure, std::min(1.0, 1.0 - dist / distLim))});
        } else {
            result.push_back(StrokePoint{interpolated, pressures[pos]});
        }
    }

    result.push_back(StrokePoint{points.back(), pressures.back()});

    // smoothing out the pressures for a better result
    const qsizetype windowSize{10};
    qreal pressureSum{};
    for (qsizetype pos = 0; pos < windowSize; pos++) {
        if (pos >= result.size()) {
            break;
        }

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

QList<QPointF> getStrokePolygon(const QList<StrokePoint> &points, const qreal thickness)
{
    using namespace Common::Utils::Math;
    if (points.empty()) {
        return QList<QPointF>();
    }

    const qreal dist{thickness};

    // if there is only one point, draw a circle
    if (points.size() == 1) {
        QList<QPointF> polygonPoints{};
        const QPointF radiusVector{dist, 0};

        constexpr int steps{26};
        for (int i = 0; i <= steps; i++) {
            const qreal delta{static_cast<qreal>(i) / steps};
            const QPointF point{QPointF{rotateVector(radiusVector, 2 * PI * delta) + points.back().point}};
            polygonPoints.push_back(point);
        }
        return polygonPoints;
    }

    QList<QPointF> leftPoints{};
    QList<QPointF> rightPoints{};

    const auto insertCap = [&](const StrokePoint &prev, const StrokePoint &cur, const StrokePoint &next) -> bool {
        const QPointF vector{next.point - cur.point};
        const qreal thickness{dist * cur.pressure};

        // if it's a sharp angle, we draw a semi circular cap (looks good)
        const QPointF prevVector{cur.point - prev.point};
        const qreal curAngle{angle(vector, prevVector)};
        if (PI / 2.0 < curAngle && curAngle < PI * 3.0 / 2.0) {
            QPointF radiusVector{unitVector(QPointF{prevVector.y(), -prevVector.x()}) * thickness};
            const qreal arcAngle{5.0 * PI / 6.0};

            constexpr int steps{13};
            for (int i = 0; i <= steps; i++) {
                const qreal delta{static_cast<qreal>(i) / steps};
                const QPointF point{QPointF{rotateVector(radiusVector, arcAngle * delta) + cur.point}};
                leftPoints.push_back(point);
            }

            if (dotProduct(prevVector, vector) == -1) {
                radiusVector = rotateVector(radiusVector, -(PI - arcAngle - PI / 18));
            } else {
                radiusVector = rotateVector(radiusVector, PI - arcAngle - PI / 18);
            }

            for (int i = 0; i <= steps; i++) {
                const qreal delta{static_cast<qreal>(i) / steps};
                const QPointF point{QPointF{rotateVector(-radiusVector, -arcAngle * delta) + cur.point}};
                rightPoints.push_back(point);
            }

            return true;
        }

        return false;
    };

    const auto insertRegularPoint = [&](const StrokePoint &prev, const StrokePoint &cur, const StrokePoint &next) -> void {
        const qreal thickness{dist * cur.pressure};
        const QPointF vector{unitVector(next.point - cur.point)};
        const QPointF prevVector{unitVector(cur.point - prev.point)};

        const QPointF lerped{lerp(vector, prevVector, dotProduct(vector, prevVector))};
        const QPointF perp{QPointF{lerped.y(), -lerped.x()} * thickness};

        leftPoints.push_back(cur.point + perp);
        rightPoints.push_back(cur.point - perp);
    };

    // inserting points
    insertRegularPoint(points[0], points[0], points[1]);
    for (qsizetype pos = 1; pos < points.size() - 1; pos++) {
        if (!insertCap(points[pos - 1], points[pos], points[pos + 1])) {
            insertRegularPoint(points[pos - 1], points[pos], points[pos + 1]);
        }
    }
    insertRegularPoint(points.back(), points.back(), *std::prev(points.end(), 2));
    std::swap(leftPoints.back(), rightPoints.back());

    constexpr int steps{26};

    // drawing the end cap
    {
        const QPointF radiusVector{leftPoints.back() - points.back().point};
        for (int i = 0; i <= steps; i++) {
            const qreal delta{static_cast<qreal>(i) / steps};
            const QPointF point{QPointF{rotateVector(radiusVector, PI * delta) + points.back().point}};
            leftPoints.push_back(point);
        }
    }

    std::reverse(rightPoints.begin(), rightPoints.end());

    // drawing the start cap
    {
        const QPointF radiusVector{rightPoints.back() - points.front().point};
        for (int i = 0; i <= steps; i++) {
            const qreal delta{static_cast<qreal>(i) / steps};
            const QPointF point{QPointF{rotateVector(radiusVector, PI * delta) + points.front().point}};
            rightPoints.push_back(point);
        }
    }

    return leftPoints + rightPoints;
}

QPainterPath getStrokePath(const QList<QPointF> &points)
{
    if (points.isEmpty()) {
        return {};
    }

    QPainterPath path{};
    path.moveTo(points[0]);

    for (qsizetype pos = 0; pos < points.size(); pos++) {
        const QPointF curPoint{points[pos]};
        const QPointF nextPoint{points[(pos + 1) % points.size()]};
        const QPointF midPoint{(curPoint + nextPoint) / 2.0};

        path.quadTo(curPoint, midPoint);
    }

    path.closeSubpath();
    path.setFillRule(Qt::WindingFill);

    return path;
}

QPainterPath getStrokeOutline(const QList<StrokePoint> &points)
{
    QPainterPath path;
    path.moveTo(points[0].point);

    for (qsizetype pos = 0; pos < points.size() - 1; pos++) {
        const QPointF curPoint{points[pos].point};
        const QPointF nextPoint{points[(pos + 1) % points.size()].point};
        const QPointF midPoint{(curPoint + nextPoint) / 2.0};

        path.quadTo(curPoint, midPoint);
    }

    path.lineTo(points.back().point);

    return path;
}
} // namespace Common::Utils::Freehand
