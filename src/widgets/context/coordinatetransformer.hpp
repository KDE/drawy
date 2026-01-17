// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QRect>
class ApplicationContext;
class SpatialContext;
class RenderingContext;

/*
 * There are three coordinate systems in drawly:
 *  1. World (the one used by the QuadTree to store items)
 *  2. Grid (the one used by the CacheGrid to cache tiles)
 *  3. View (the viewport) */
class CoordinateTransformer
{
private:
    SpatialContext *m_spatialContext{};
    RenderingContext *m_renderingContext{};
    ApplicationContext *m_applicationContext;

public:
    explicit CoordinateTransformer(ApplicationContext *context);
    ~CoordinateTransformer();

    void setCoordinateTransformer();

    [[nodiscard]] QPointF worldToView(QPointF point);
    [[nodiscard]] QSizeF worldToView(QSizeF size);
    [[nodiscard]] QRectF worldToView(QRectF rect);
    [[nodiscard]] QPointF viewToWorld(QPointF point);
    [[nodiscard]] QSizeF viewToWorld(QSizeF size);
    [[nodiscard]] QRectF viewToWorld(QRectF rect);

    [[nodiscard]] QPoint worldToView(QPoint point);
    [[nodiscard]] QSize worldToView(QSize size);
    [[nodiscard]] QRect worldToView(QRect rect);
    [[nodiscard]] QPoint viewToWorld(QPoint point);
    [[nodiscard]] QSize viewToWorld(QSize size);
    [[nodiscard]] QRect viewToWorld(QRect rect);

    [[nodiscard]] QPointF worldToGrid(QPointF point);
    [[nodiscard]] QSizeF worldToGrid(QSizeF size);
    [[nodiscard]] QRectF worldToGrid(QRectF rect);
    [[nodiscard]] QPointF gridToWorld(QPointF point);
    [[nodiscard]] QSizeF gridToWorld(QSizeF size);
    [[nodiscard]] QRectF gridToWorld(QRectF rect);

    [[nodiscard]] QPoint worldToGrid(QPoint point);
    [[nodiscard]] QSize worldToGrid(QSize size);
    [[nodiscard]] QRect worldToGrid(QRect rect);
    [[nodiscard]] QPoint gridToWorld(QPoint point);
    [[nodiscard]] QSize gridToWorld(QSize size);
    [[nodiscard]] QRect gridToWorld(QRect rect);

    [[nodiscard]] QPointF viewToGrid(QPointF point);
    [[nodiscard]] QSizeF viewToGrid(QSizeF size);
    [[nodiscard]] QRectF viewToGrid(QRectF rect);
    [[nodiscard]] QPointF gridToView(QPointF point);
    [[nodiscard]] QSizeF gridToView(QSizeF size);
    [[nodiscard]] QRectF gridToView(QRectF rect);

    [[nodiscard]] QPoint viewToGrid(QPoint point);
    [[nodiscard]] QSize viewToGrid(QSize size);
    [[nodiscard]] QRect viewToGrid(QRect rect);
    [[nodiscard]] QPoint gridToView(QPoint point);
    [[nodiscard]] QSize gridToView(QSize size);
    [[nodiscard]] QRect gridToView(QRect rect);

    [[nodiscard]] QPoint round(QPointF point);
    [[nodiscard]] QRect round(QRectF rect);
    [[nodiscard]] QSize round(QSizeF size);
};
