#include "freehand.hpp"

namespace Common::Utils::Freehand {
QList<QPointF> getStrokePoints(const QList<QPointF> &points) {
    if (points.size() <= 1) {
        return points;
    }

    QList<QPointF> result{};
    for (const QPointF &point : points) {
        if (result.empty()) {
            result.push_back(point);
            continue;
        }

        result.push_back(result.back() + t * (point - result.back()));
    }

    result.push_back(points.back());

    return result;
}
};
