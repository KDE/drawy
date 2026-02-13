// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "coordinatetransformer.hpp"

#include <cmath>

#include "applicationcontext.hpp"
#include "drawy_debug.h"
#include "renderingcontext.hpp"
#include "spatialcontext.hpp"

CoordinateTransformer::CoordinateTransformer(ApplicationContext *context)
    : m_applicationContext{context}
{
}

CoordinateTransformer::~CoordinateTransformer()
{
    qCDebug(DRAWY_LOG) << "Object deleted: CoordinateTransformer";
}

void CoordinateTransformer::setCoordinateTransformer()
{
    m_spatialContext = m_applicationContext->spatialContext();
    m_renderingContext = m_applicationContext->renderingContext();
}

QPointF CoordinateTransformer::worldToView(QPointF point)
{
    return (point - m_spatialContext->offsetPos()) * m_renderingContext->zoomFactor();
}

QSizeF CoordinateTransformer::worldToView(QSizeF size)
{
    return size * m_renderingContext->zoomFactor();
}

QRectF CoordinateTransformer::worldToView(QRectF rect)
{
    return {worldToView(rect.topLeft()), worldToView(rect.size())};
}

QPolygonF CoordinateTransformer::worldToView(QPolygonF rect)
{
    for (QPointF &point : rect) {
        point = worldToView(point);
    }

    return rect;
}

QPointF CoordinateTransformer::viewToWorld(QPointF point)
{
    return point / m_renderingContext->zoomFactor() + m_spatialContext->offsetPos();
}

QSizeF CoordinateTransformer::viewToWorld(QSizeF size)
{
    return size / m_renderingContext->zoomFactor();
}

QRectF CoordinateTransformer::viewToWorld(QRectF rect)
{
    return {viewToWorld(rect.topLeft()), viewToWorld(rect.size())};
}

QPolygonF CoordinateTransformer::viewToWorld(QPolygonF rect)
{
    for (QPointF &point : rect) {
        point = viewToWorld(point);
    }

    return rect;
}

QPoint CoordinateTransformer::worldToView(QPoint point)
{
    return round(worldToView(point.toPointF()));
}

QSize CoordinateTransformer::worldToView(QSize size)
{
    return round(worldToView(size.toSizeF()));
}

QRect CoordinateTransformer::worldToView(QRect rect)
{
    return round(worldToView(rect.toRectF()));
}

QPolygon CoordinateTransformer::worldToView(QPolygon rect)
{
    for (QPoint &point : rect) {
        point = worldToView(point);
    }

    return rect;
}

QPoint CoordinateTransformer::viewToWorld(QPoint point)
{
    return round(viewToWorld(point.toPointF()));
}

QSize CoordinateTransformer::viewToWorld(QSize size)
{
    return round(viewToWorld(size.toSizeF()));
}

QRect CoordinateTransformer::viewToWorld(QRect rect)
{
    return round(viewToWorld(rect.toRectF()));
}

QPolygon CoordinateTransformer::viewToWorld(QPolygon rect)
{
    for (QPoint &point : rect) {
        point = viewToWorld(point);
    }

    return rect;
}

QPointF CoordinateTransformer::worldToGrid(QPointF point)
{
    return point * m_renderingContext->zoomFactor();
}

QSizeF CoordinateTransformer::worldToGrid(QSizeF size)
{
    return size * m_renderingContext->zoomFactor();
}

QRectF CoordinateTransformer::worldToGrid(QRectF rect)
{
    return {worldToGrid(rect.topLeft()), worldToGrid(rect.size())};
}

QPolygonF CoordinateTransformer::worldToGrid(QPolygonF rect)
{
    for (QPointF &point : rect) {
        point = worldToGrid(point);
    }

    return rect;
}

QPointF CoordinateTransformer::gridToWorld(QPointF point)
{
    return point / m_renderingContext->zoomFactor();
}

QSizeF CoordinateTransformer::gridToWorld(QSizeF size)
{
    return size / m_renderingContext->zoomFactor();
}

QRectF CoordinateTransformer::gridToWorld(QRectF rect)
{
    return {gridToWorld(rect.topLeft()), gridToWorld(rect.size())};
}

QPolygonF CoordinateTransformer::gridToWorld(QPolygonF rect)
{
    for (QPointF &point : rect) {
        point = gridToWorld(point);
    }

    return rect;
}

QPoint CoordinateTransformer::worldToGrid(QPoint point)
{
    return round(worldToGrid(point.toPointF()));
}

QSize CoordinateTransformer::worldToGrid(QSize size)
{
    return round(worldToGrid(size.toSizeF()));
}

QRect CoordinateTransformer::worldToGrid(QRect rect)
{
    return round(worldToGrid(rect.toRectF()));
}

QPolygon CoordinateTransformer::worldToGrid(QPolygon rect)
{
    for (QPoint &point : rect) {
        point = worldToGrid(point);
    }

    return rect;
}

QPoint CoordinateTransformer::gridToWorld(QPoint point)
{
    return round(gridToWorld(point.toPointF()));
}

QSize CoordinateTransformer::gridToWorld(QSize size)
{
    return round(gridToWorld(size.toSizeF()));
}

QRect CoordinateTransformer::gridToWorld(QRect rect)
{
    return round(gridToWorld(rect.toRectF()));
}

QPolygon CoordinateTransformer::gridToWorld(QPolygon rect)
{
    for (QPoint &point : rect) {
        point = gridToWorld(point);
    }

    return rect;
}

QPointF CoordinateTransformer::viewToGrid(QPointF point)
{
    return point + worldToView(m_spatialContext->offsetPos());
}

QSizeF CoordinateTransformer::viewToGrid(QSizeF size)
{
    return size;
}

QRectF CoordinateTransformer::viewToGrid(QRectF rect)
{
    return QRectF{viewToGrid(rect.topLeft()), viewToGrid(rect.bottomRight())};
}

QPolygonF CoordinateTransformer::viewToGrid(QPolygonF rect)
{
    for (QPointF &point : rect) {
        point = viewToGrid(point);
    }

    return rect;
}

QPointF CoordinateTransformer::gridToView(QPointF point)
{
    return point - worldToGrid(m_spatialContext->offsetPos());
}

QSizeF CoordinateTransformer::gridToView(QSizeF size)
{
    return size;
}

QRectF CoordinateTransformer::gridToView(QRectF rect)
{
    return QRectF{gridToView(rect.topLeft()), gridToView(rect.bottomRight())};
}

QPolygonF CoordinateTransformer::gridToView(QPolygonF rect)
{
    for (QPointF &point : rect) {
        point = gridToView(point);
    }

    return rect;
}

QPoint CoordinateTransformer::viewToGrid(QPoint point)
{
    return round(viewToGrid(point.toPointF()));
}

QSize CoordinateTransformer::viewToGrid(QSize size)
{
    return round(viewToGrid(size.toSizeF()));
}

QRect CoordinateTransformer::viewToGrid(QRect rect)
{
    return round(viewToGrid(rect.toRectF()));
}

QPolygon CoordinateTransformer::viewToGrid(QPolygon rect)
{
    for (QPoint &point : rect) {
        point = viewToGrid(point);
    }

    return rect;
}

QPoint CoordinateTransformer::gridToView(QPoint point)
{
    return round(gridToView(point.toPointF()));
}

QSize CoordinateTransformer::gridToView(QSize size)
{
    return round(gridToView(size.toSizeF()));
}

QRect CoordinateTransformer::gridToView(QRect rect)
{
    return round(gridToView(rect.toRectF()));
}

QPolygon CoordinateTransformer::gridToView(QPolygon rect)
{
    for (QPoint &point : rect) {
        point = gridToView(point);
    }

    return rect;
}

QPoint CoordinateTransformer::round(QPointF point)
{
    return QPoint{qRound(point.x()), qRound(point.y())};
}

QRect CoordinateTransformer::round(QRectF rect)
{
    const int x1 = qRound(rect.left());
    const int y1 = qRound(rect.top());
    const int x2 = qRound(rect.right());
    const int y2 = qRound(rect.bottom());

    return QRect{x1, y1, x2 - x1, y2 - y1};
}

QSize CoordinateTransformer::round(QSizeF size)
{
    return QSize{qRound(size.width()), qRound(size.height())};
}

QPolygon CoordinateTransformer::round(QPolygonF rect)
{
    for (QPointF &point : rect) {
        point = round(point);
    }

    return rect.toPolygon();
}
