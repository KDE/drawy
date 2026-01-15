#pragma once

#include <QList>
#include <QPoint>

namespace Common::Utils::Freehand {
/**
 * @brief: smoothness factor, the lower it is, the smoother the lines are but they look more unnatural
 */
inline constexpr qreal t = 0.575;

/**
 * @brief smoothens a stroke formed from a list of points
 * @param points a QList of QPointF
 * @returns a QList of QPointF with the same number of points but with less noise and smoother
 */
QList<QPointF> getStrokePoints(const QList<QPointF> &points);
};
