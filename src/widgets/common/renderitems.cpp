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
#include "data-structures/quadtree.hpp"
#include "item/item.hpp"

// TODO: Refactor this
void Common::renderCanvas(ApplicationContext *context)
{
    CoordinateTransformer &transformer{context->spatialContext().coordinateTransformer()};
    Canvas &canvas{context->renderingContext().canvas()};
    QPointF offsetPos{context->spatialContext().offsetPos()};

    canvas.setCanvasBg(canvas.canvasBg());

    QRectF viewport(offsetPos, transformer.viewToWorld(canvas.dimensions()));
    QVector<std::shared_ptr<Item>> intersectingItems{context->spatialContext().quadtree().queryItems(viewport, []([[maybe_unused]] auto &a, [[maybe_unused]] auto &b) {
        return true;
    })};

    for (const auto &item : intersectingItems) {
        context->renderingContext().canvas().paintCanvas([&](QPainter &painter) {
            const qreal zoomFactor{context->renderingContext().zoomFactor()};
            painter.scale(zoomFactor, zoomFactor);
            item->draw(painter, offsetPos);
        });
    }

    // for (const auto &cell : visibleCells) {
    //     // canvasPainter.save();
    //     // QPen pen; pen.setColor(Qt::white); canvasPainter.setPen(pen);
    //     // canvasPainter.drawRect(transformer.gridToView(cell->rect()));
    //     // canvasPainter.restore();

    //     if (cell->dirty()) {
    //         cell->setDirty(false);

    //         QVector<std::shared_ptr<Item>> intersectingItems{
    //             context->spatialContext()->quadtree()->queryItems(transformer->gridToWorld(cell->rect()), [](const auto &a, auto b) {
    //                 return true;
    //             })};

    //         if (intersectingItems.empty())
    //             continue;

    //         const qreal zoomFactor{context->renderingContext()->zoomFactor()};

    //         const QPointF topLeftPoint{transformer->gridToWorld(cell->rect().topLeft().toPointF())};

    //         cell->paintImage([&](QPainter& painter){
    //             painter.resetTransform();
    //             painter.scale(zoomFactor, zoomFactor);

    //             for (const auto &intersectingItem : intersectingItems) {
    //                 intersectingItem->draw(painter, topLeftPoint);
    //             }
    //         });
    //     }

    //     context->renderingContext()->canvas()->paintCanvas([&](QPainter& painter){
    //         painter.drawImage(transformer->round(transformer->gridToView(cell->rect())), cell->image()->toImage());
    //     });
    // }

    auto selectedItems{context->selectionContext().selectedItems()};

    if (selectedItems.empty())
        return;

    QRectF selectionBox{};
    // render a box around selected items
    QPen pen{Common::selectionBorderColor};
    pen.setWidth(2);

    context->renderingContext().canvas().paintCanvas([&](QPainter &painter) {
        painter.setPen(pen);

        for (const auto &item : selectedItems) {
            const QRectF curBox{transformer.worldToView(item->boundingBox()).normalized()};
            painter.drawRect(curBox);
            selectionBox |= curBox;
        }

        painter.setPen(pen);
        painter.drawRect(selectionBox);
    });
}
