// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "freehand.hpp"
#include "drawyglobalconfig.h"
#include "math.hpp"

namespace Common::Utils::Freehand
{
void updateSettings()
{
    streamline = DrawyGlobalConfig::self()->streamline();
    thinning = DrawyGlobalConfig::self()->thinning();
}

QPainterPath getStroke(const QList<QPointF> &points, const QList<qreal> &pressures, const bool simulatePressure, const qreal thickness)
{
    return getStrokePath(getStrokePolygon(getStrokePoints(points, pressures, simulatePressure, thickness), thickness));
}

QPainterPath getStroke(const QList<QPointF> &points, const QList<qreal> &pressures, const bool simulatePressure, const qreal thickness, qreal &lastPressure)
{
    const auto strokePoints{getStrokePoints(points, pressures, simulatePressure, thickness)};

    if (!strokePoints.empty()) {
        lastPressure = strokePoints.back().pressure;
    }

    return getStrokePath(getStrokePolygon(strokePoints, thickness));
}

QList<StrokePoint> getStrokePoints(const QList<QPointF> &points, const QList<qreal> &pressures, const bool simulatePressure, const qreal thickness)
{
    using namespace Common::Utils::Math;
    if (points.size() != pressures.size()) {
        throw std::logic_error("Pressures and points list have different sizes");
    }

    if (points.empty()) {
        return QList<StrokePoint>();
    }

    QList<StrokePoint> result;
    result.push_back(StrokePoint{points[0], pressures[0]});

    if (points.size() == 1) {
        return result;
    }

    const qreal minLength{thickness};
    qreal runningLength{0};

    for (qsizetype pos = 1; pos < points.size(); pos++) {
        const QPointF prev{result.back().point};
        const QPointF interpolated{lerp(prev, points[pos], t)};

        if (interpolated == prev) {
            continue;
        }

        const qreal dist{Math::dist(interpolated, prev)};
        runningLength += dist;

        if (runningLength < minLength) {
            continue;
        }

        if (simulatePressure) {
            const qreal speed{qMin(1.0, dist / thickness)};
            const qreal change{qMin(1.0, 1 - speed)};

            const qreal press{qMin(1.0, result.back().pressure + (change - result.back().pressure) * (speed * 0.275))};

            result.push_back(StrokePoint{interpolated, press});
        } else {
            result.push_back(StrokePoint{interpolated, pressures[pos]});
        }
    }

    qreal reverseRunningLength{0};

    // removing noise from the end
    while (result.size() > 1) {
        const qreal dist{Math::dist(result.back().point, points.back())};
        reverseRunningLength += dist;

        if (reverseRunningLength < minLength) {
            result.pop_back();
        } else {
            break;
        }
    }

    if (result.size() > 1 || Math::dist(points.back(), result.back().point) >= minLength) {
        result.push_back(StrokePoint{points.back(), pressures.back()});
    }

    // smoothing out the pressures for a better result
    constexpr qsizetype windowSize{10};
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
        QList<QPointF> polygonPoints;
        const QPointF radiusVector{getStrokeRadius(thickness, points[0].pressure), 0};

        constexpr int steps{26};
        for (int i = 0; i <= steps; i++) {
            const qreal delta{static_cast<qreal>(i) / steps};
            const QPointF point{QPointF{rotateVector(radiusVector, 2 * PI * delta) + points.back().point}};
            polygonPoints.push_back(point);
        }
        return polygonPoints;
    }

    QList<QPointF> leftPoints;
    QList<QPointF> rightPoints;

    const auto insertCap = [&](const StrokePoint &prev, const StrokePoint &cur, const StrokePoint &next) -> bool {
        const QPointF prevVector{cur.point - prev.point};
        const QPointF nextVector{next.point - cur.point};

        if (Math::dist(prevVector, QPointF{0, 0}) == 0.0 || Math::dist(nextVector, QPointF{0, 0}) == 0.0) {
            return false;
        }

        const QPointF prevUnit{unitVector(prevVector)};
        const QPointF nextUnit{unitVector(nextVector)};

        const qreal dot = dotProduct(prevUnit, nextUnit);

        // Sharp turn detected
        if (dot < 0.0) {
            const qreal radius{getStrokeRadius(dist, cur.pressure)};

            const qreal sweepAngle = qAcos(std::max(-1.0, std::min(1.0, dot)));

            const qreal cross = prevUnit.x() * nextUnit.y() - prevUnit.y() * nextUnit.x();
            const bool isRightTurn = (cross > 0);

            const QPointF prevLeftNormal{prevUnit.y(), -prevUnit.x()};
            const QPointF prevRightNormal{-prevUnit.y(), prevUnit.x()};

            constexpr int steps{13};

            if (isRightTurn) {
                const QPointF startRadiusVector = prevLeftNormal * radius;

                for (int i = 0; i <= steps; i++) {
                    const qreal delta = static_cast<qreal>(i) / steps;
                    const QPointF rotated = rotateVector(startRadiusVector, sweepAngle * delta);
                    leftPoints.push_back(cur.point + rotated);
                }

                rightPoints.push_back(cur.point);
            } else {
                leftPoints.push_back(cur.point);

                const QPointF startRadiusVector = prevRightNormal * radius;

                for (int i = 0; i <= steps; i++) {
                    const qreal delta = static_cast<qreal>(i) / steps;
                    const QPointF rotated = rotateVector(startRadiusVector, -sweepAngle * delta);
                    rightPoints.push_back(cur.point + rotated);
                }
            }

            return true;
        }

        return false;
    };

    // we want the rate of growth of minDistance to slow down
    // as thickness increases
    const qreal exponent{3 - 0.2 * thickness};
    const qreal minDistance{qPow(qMax(1.0, thickness * smoothing), exponent)};

    const auto insertRegularPoint = [&](const StrokePoint &prev, const StrokePoint &cur, const StrokePoint &next, bool isLast = false) -> void {
        const qreal thickness{getStrokeRadius(dist, cur.pressure)};
        const QPointF vector{unitVector(next.point - cur.point)};
        const QPointF prevVector{unitVector(cur.point - prev.point)};

        const QPointF lerped{lerp(vector, prevVector, dotProduct(vector, prevVector))};
        const QPointF perp{QPointF{lerped.y(), -lerped.x()} * thickness};

        const auto newLeftPoint{cur.point + perp};
        if (isLast || leftPoints.empty() || Math::dist(leftPoints.back(), newLeftPoint) >= minDistance) {
            leftPoints.push_back(newLeftPoint);
        }

        const auto newRightPoint{cur.point - perp};
        if (isLast || rightPoints.empty() || Math::dist(rightPoints.back(), newRightPoint) >= minDistance) {
            rightPoints.push_back(newRightPoint);
        }
    };

    // inserting points
    insertRegularPoint(points[0], points[0], points[1]);
    for (qsizetype pos = 1; pos < points.size() - 1; pos++) {
        if (!insertCap(points[pos - 1], points[pos], points[pos + 1])) {
            insertRegularPoint(points[pos - 1], points[pos], points[pos + 1]);
        }
    }
    insertRegularPoint(points.back(), points.back(), *std::prev(points.end(), 2), true);
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

    QPainterPath path;
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

[[nodiscard]] qreal getStrokeRadius(const qreal thickness, const qreal pressure)
{
    return thickness * (0.5 - thinning * (0.5 - pressure));
}
} // namespace Common::Utils::Freehand
