// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "renderitems.hpp"

#include <QPointF>
#include <QRectF>
#include <utility>

#include "canvas/canvas.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "item/itemcache/itemcache.hpp"

void Common::renderCanvas(ApplicationContext *context)
{
    auto &spatial{*context->spatialContext()};
    auto &rendering{*context->renderingContext()};
    auto &transformer{spatial.coordinateTransformer()};
    auto *canvas{rendering.canvas()};

    const QPointF offsetPos{spatial.offsetPos()};
    canvas->setCanvasBg(canvas->canvasBg());

    const QPointF gridOffset{transformer.worldToGrid(offsetPos)};
    const QRectF gridViewport{gridOffset, transformer.viewToGrid(canvas->dimensions())};

    const auto visibleCells{rendering.cacheGrid().queryCells(transformer.round(gridViewport))};

    for (const auto &cell : std::as_const(visibleCells)) {
        // DEBUG VISUALIZATION
        // canvas->paintCanvas([&](QPainter &painter) {
        //     painter.setPen(QPen{Qt::white});
        //     painter.drawRect(transformer.gridToView(cell->rect()));
        // });

        if (cell->dirty()) {
            cell->pixmap().fill(Qt::transparent);
            cell->setDirty(false);

            const auto intersectingItems{spatial.quadtree().queryItems(transformer.gridToWorld(cell->rect()), [](auto &, auto &) {
                return true;
            })};

            if (intersectingItems.empty()) {
                continue;
            }

            const qreal zoom{rendering.zoomFactor()};
            const QRectF cellRectF{cell->rect().toRectF()};
            const QPointF topLeft{transformer.gridToWorld(cellRectF.topLeft())};

            cell->paint([&](QPainter &painter) {
                for (const auto &item : std::as_const(intersectingItems)) {
                    if (item->needsCaching()) {
                        rendering.itemCache().drawCached(painter, item, transformer.gridToWorld(cellRectF), cellRectF.topLeft());
                    } else {
                        painter.save();
                        painter.scale(zoom, zoom);

                        QTransform cell{};
                        cell.translate(topLeft.x(), topLeft.y());
                        painter.setTransform(cell.inverted(), true);

                        painter.setTransform(item->transformObj(), true);
                        item->draw(painter, QPointF{0, 0});
                        painter.restore();
                    }
                }
            });
        }

        canvas->paintCanvas([&](QPainter &painter) {
            painter.drawPixmap(transformer.round(transformer.gridToView(cell->rect())), cell->pixmap());
        });
    }

    context->selectionContext()->renderHandles();
}
