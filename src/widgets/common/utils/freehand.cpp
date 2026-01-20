#include "freehand.hpp"
#include <algorithm>
#include <kstandardguiitem.h>
#include <qbrush.h>
#include <qbytearrayview.h>
#include <qdebug.h>
#include <qmath.h>

namespace Common::Utils::Freehand {
QList<StrokePoint> getStrokePoints(const QList<QPointF> &points,
                                   const QList<qreal> &pressures) {
  if (points.size() != pressures.size()) {
    throw new std::logic_error(
        "Pressures and points list have different sizes");
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
  for (qsizetype pos = 1; pos < points.size(); pos++) {
    const QPointF prev{result.back().point};
    QPointF interpolated{prev + t * (points[pos] - prev)};

    const qreal dist{length(interpolated - prev)};
    if (dist < minDist)
      continue;

    result.push_back(StrokePoint{interpolated, pressures[pos]});
  }

  result.push_back(StrokePoint{points.back(), pressures.back()});
  return result;
}

QList<QPointF> getStrokePolygon(const QList<StrokePoint> &points) {
  if (points.empty()) {
    return {};
  }

  QList<QPointF> polygonPoints{};
  const qreal dist{5};

  // if there is only one point, draw a circle
  if (points.size() == 1) {
    const QPointF radiusVector{dist, 0};
    for (qreal delta = 0, step = 1.0 / 26; delta <= 1; delta += step) {
      const QPointF point{QPointF{rotateVector(radiusVector, 2 * PI * delta) +
                                  points.back().point}};
      polygonPoints.push_back(point);
    }
    return polygonPoints;
  }

  // we do this twice so it's better to turn this into a lambda
  const auto insertCap = [dist](const StrokePoint &prev, const StrokePoint &cur,
                                const StrokePoint &next,
                                QList<QPointF> &polygon) -> bool {
    const QPointF vector{next.point - cur.point};
    const qreal thickness{dist * cur.pressure};

    // if it's a sharp angle, we draw a semi circular cap (looks good)
    const QPointF prevVector{cur.point - prev.point};
    const qreal curAngle{angle(vector, prevVector)};
    if (curAngle > PI / 2 && !polygon.empty()) {
      QPointF radiusVector{
          unitVector(QPointF{prevVector.y(), -prevVector.x()}) * thickness};
      for (qreal delta = 0, step = 1.0 / 13; delta <= 1; delta += step) {
        const QPointF point{
            QPointF{rotateVector(radiusVector, PI * delta) + cur.point}};
        const QPointF interpolated{polygon.back() +
                                   0.575 * (point - polygon.back())};
        polygon.push_back(interpolated);
      }

      return true;
    }

    return false;
  };

  const auto insertRegularPoint =
      [dist](const StrokePoint &cur, const StrokePoint &next,
             QList<QPointF> &polygon, const bool flip = false,
             const bool interpolate = true) -> void {
    const QPointF vector{next.point - cur.point};
    const qreal thickness{dist * cur.pressure};

    QPointF perp{unitVector(QPointF{vector.y(), -vector.x()})};
    if (flip) {
      perp *= -1;
    }

    const QPointF perpPoint{cur.point + perp * thickness};
    if (!interpolate || polygon.empty()) {
      polygon.push_back(QPointF{cur.point + perp * thickness});
    } else {
      const QPointF interpolated{polygon.back() +
                                 0.575 * (perpPoint - polygon.back())};
      polygon.push_back(interpolated);
    }
  };

  insertRegularPoint(points[0], points[1], polygonPoints, false, false);
  for (qsizetype pos = 1; pos < points.size() - 1; pos++) {
    if (!insertCap(points[pos - 1], points[pos], points[pos + 1],
                   polygonPoints)) {
      insertRegularPoint(points[pos], points[pos + 1], polygonPoints);
    }
  }
  insertRegularPoint(points.back(), *std::prev(points.end(), 2), polygonPoints,
                     true, false);

  // drawing the end cap
  {
    const QPointF radiusVector{polygonPoints.back() - points.back().point};
    for (qreal delta = 0, step = 1.0 / 26; delta <= 1; delta += step) {
      const QPointF point{QPointF{rotateVector(radiusVector, PI * delta) +
                                  points.back().point}};
      polygonPoints.push_back(point);
    }
  }

  insertRegularPoint(points.back(), *std::prev(points.end(), 2), polygonPoints,
                     false, false);
  for (qsizetype pos = points.size() - 2; pos >= 1; pos--) {
    if (!insertCap(points[pos + 1], points[pos], points[pos - 1],
                   polygonPoints)) {
      insertRegularPoint(points[pos], points[pos - 1], polygonPoints);
    }
  }
  insertRegularPoint(points[0], points[1], polygonPoints, true, false);

  // drawing the start cap
  {
    const QPointF radiusVector{polygonPoints.back() - points.front().point};
    for (qreal delta = 0, step = 1.0 / 26; delta <= 1; delta += step) {
      const QPointF point{QPointF{rotateVector(radiusVector, PI * delta) +
                                  points.front().point}};
      polygonPoints.push_back(point);
    }
  }

  return polygonPoints;
}

qreal length(const QPointF &point) {
  const qreal x{point.x()};
  const qreal y{point.y()};
  return qSqrt(x * x + y * y);
}

QPointF unitVector(const QPointF &point) {
  const qreal len{length(point)};
  if (len == 0) {
    return point;
  }

  return QPointF{point.x() / len, point.y() / len};
}

QPointF rotateVector(const QPointF &vector, const qreal angle) {
  qreal cosTheta{qCos(angle)};
  qreal sinTheta{qSin(angle)};

  return QPointF{vector.x() * cosTheta - vector.y() * sinTheta,
                 vector.x() * sinTheta + vector.y() * cosTheta};
}

qreal dotProduct(const QPointF &vectorA, const QPointF &vectorB) {
  return vectorA.x() * vectorB.x() + vectorA.y() * vectorB.y();
}

qreal angle(const QPointF &vectorA, const QPointF &vectorB) {
  const qreal lenA{length(vectorA)};
  const qreal lenB{length(vectorB)};
  const qreal dot{dotProduct(vectorA, vectorB)};

  return qAcos(dot / (lenA * lenB));
}

int crossProduct(const QPointF &vectorA, const QPointF &vectorB) {
  return ((vectorA.x() * vectorB.y() - vectorB.x() * vectorA.y()) > 0 ? 1 : -1);
}
};
