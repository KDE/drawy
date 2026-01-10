// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "renderitems.hpp"

#include <QPointF>
#include <QRectF>
#include <memory>

#include "canvas/canvas.hpp"
#include "constants.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "item/item.hpp"
#include "item/itemcache/itemcache.hpp"

// TODO: Refactor this
void Common::renderCanvas(ApplicationContext *context)
{
    CoordinateTransformer &transformer{context->spatialContext().coordinateTransformer()};
    Canvas &canvas{context->renderingContext().canvas()};
    QPointF offsetPos{context->spatialContext().offsetPos()};

    canvas.setCanvasBg(canvas.canvasBg());

    QPointF gridOffset{transformer.worldToGrid(offsetPos)};
    QRectF gridViewport(gridOffset, transformer.viewToGrid(canvas.dimensions()));

    QList<std::shared_ptr<CacheCell>> visibleCells{context->renderingContext().cacheGrid().queryCells(transformer.round(gridViewport))};

    for (const auto &cell : visibleCells) {
        context->renderingContext().canvas().paintCanvas([&](QPainter &painter) -> void {
            QPen pen;
            pen.setColor(Qt::white);
            painter.setPen(pen);
            painter.drawRect(transformer.gridToView(cell->rect()));
        });

        if (cell->dirty()) {
            cell->pixmap().fill(Qt::transparent);
            cell->setDirty(false);

            QList<std::shared_ptr<Item>> intersectingItems{
                context->spatialContext().quadtree().queryItems(transformer.gridToWorld(cell->rect()), []([[maybe_unused]] auto &a, [[maybe_unused]] auto &b) {
                    return true;
                })};

            if (intersectingItems.empty())
                continue;

            const qreal zoomFactor{context->renderingContext().zoomFactor()};
            const QPointF topLeftPoint{transformer.gridToWorld(cell->rect().topLeft().toPointF())};

                for (const auto &intersectingItem : intersectingItems) {
                    if (intersectingItem->needsCaching()) {
                        cell->paint([&](QPainter &painter) -> void {
                            context->renderingContext().itemCache().drawCached(painter, intersectingItem, cell->rect().topLeft().toPointF());
                        });
                    } else {
                        cell->paint([&](QPainter &painter) -> void {
                            painter.scale(zoomFactor, zoomFactor);
                            intersectingItem->draw(painter, topLeftPoint);
                        });
                    }
                }
        }

        context->renderingContext().canvas().paintCanvas([&](QPainter &painter) -> void {
            painter.drawPixmap(transformer.round(transformer.gridToView(cell->rect())), cell->pixmap());
        });
    }

    auto selectedItems{context->selectionContext().selectedItems()};

    if (selectedItems.empty())
        return;

    // render a box around selected items
    context->renderingContext().canvas().paintCanvas([&](QPainter &painter) -> void {
        QPen pen{Common::selectionBorderColor};
        pen.setWidth(2);

        painter.setPen(pen);

        QRectF selectionBox{};
        for (const auto &item : selectedItems) {
            const QRectF curBox{transformer.worldToView(item->boundingBox()).normalized()};
            painter.drawRect(curBox);
            selectionBox |= curBox;
        }

        painter.setPen(pen);
        painter.drawRect(selectionBox);
    });
}
