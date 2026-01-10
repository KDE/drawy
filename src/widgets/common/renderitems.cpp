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

// TODO: Refactor this
void Common::renderCanvas(ApplicationContext *context)
{
    CoordinateTransformer &transformer{context->spatialContext().coordinateTransformer()};
    Canvas &canvas{context->renderingContext().canvas()};
    QPointF offsetPos{context->spatialContext().offsetPos()};

    canvas.canvas()->fill(canvas.bg());

    QPointF gridOffset{transformer.worldToGrid(offsetPos)};
    QRectF gridViewport(gridOffset, transformer.viewToGrid(canvas.dimensions()));

    QList<std::shared_ptr<CacheCell>> visibleCells{context->spatialContext().cacheGrid().queryCells(transformer.round(gridViewport))};

    QPainter &canvasPainter{context->renderingContext().canvasPainter()};

    for (const auto &cell : visibleCells) {
        // canvasPainter.save();
        // QPen pen; pen.setColor(Qt::white); canvasPainter.setPen(pen);
        // canvasPainter.drawRect(transformer.gridToView(cell.rect()));
        // canvasPainter.restore();

        if (cell->dirty()) {
            cell->image()->fill(Qt::transparent);
            cell->setDirty(false);

            QList<std::shared_ptr<Item>> intersectingItems{
                context->spatialContext().quadtree().queryItems(transformer.gridToWorld(cell->rect()), []([[maybe_unused]] auto &a, [[maybe_unused]] auto &b) {
                    return true;
                })};

            if (intersectingItems.empty())
                continue;

            const qreal zoomFactor{context->renderingContext().zoomFactor()};

            const QPointF topLeftPoint{transformer.gridToWorld(cell->rect().topLeft().toPointF())};

            cell->painter()->resetTransform();
            cell->painter()->scale(zoomFactor, zoomFactor);

            for (const auto &intersectingItem : intersectingItems) {
                intersectingItem->draw(*cell->painter(), topLeftPoint);
            }
        }

        canvasPainter.drawPixmap(transformer.round(transformer.gridToView(cell->rect())), *cell->image());
    }

    auto selectedItems{context->selectionContext().selectedItems()};

    if (selectedItems.empty())
        return;

    QRectF selectionBox{};
    // render a box around selected items
    canvasPainter.save();
    QPen pen{Common::selectionBorderColor};
    pen.setWidth(2);

    canvasPainter.setPen(pen);

    for (const auto &item : selectedItems) {
        const QRectF curBox{transformer.worldToView(item->boundingBox()).normalized()};
        canvasPainter.drawRect(curBox);
        selectionBox |= curBox;
    }

    canvasPainter.setPen(pen);
    canvasPainter.drawRect(selectionBox);
    canvasPainter.restore();
}
