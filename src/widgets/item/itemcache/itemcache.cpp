// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "itemcache.hpp"
#include "../../canvas/canvas.hpp"
#include "../../common/constants.hpp"
#include "../../context/applicationcontext.hpp"
#include "../../context/coordinatetransformer.hpp"
#include "../../context/renderingcontext.hpp"
#include "../../context/spatialcontext.hpp"
#include "../../data-structures/cachegrid.hpp"
#include "../item.hpp"

void ItemCache::drawCached(QPainter &painter, const std::shared_ptr<Item> &item, const QPointF &offset)
{
    Q_ASSERT(item->needsCaching());

    auto *context{ApplicationContext::instance()};
    auto &transformer{context->spatialContext().coordinateTransformer()};
    auto &canvas{context->renderingContext().canvas()};

    QRectF boundingBox{item->boundingBox()};
    QTransform transform{item->transformObj()};
    // account for initial translation
    transform.translate(boundingBox.topLeft().x(), boundingBox.topLeft().y());

    const QPointF worldOffset{context->spatialContext().offsetPos()};
    const QRectF worldViewport(worldOffset, transformer.viewToWorld(canvas.dimensions()));
    const QRectF gridViewport{transformer.worldToGrid(worldViewport)};
    const QRectF transformedGridViewport{transformer.worldToGrid(transform.inverted().mapRect(worldViewport))};

    // Check if item is not already cached
    if (m_cacheGrids.find(item) == m_cacheGrids.end()) {
        QSizeF itemSize{transformer.worldToGrid(boundingBox.size())};
        QSizeF maxCellSize{Common::maxItemCacheCellSize.toSizeF()};

        // If the item is smaller than the max cell size, we use just one cell of
        // size equal to its bounding box
        int maxCellCount{1};
        QSize cellSize{transformer.worldToGrid(boundingBox.size().toSize())};
        if (itemSize.width() / maxCellSize.width() > maxCellSize.height() / itemSize.height()) {
            int rows{static_cast<int>(std::ceil(gridViewport.width() / maxCellSize.width())) + 1};
            int cols{static_cast<int>(std::ceil(gridViewport.height() / maxCellSize.height())) + 1};

            maxCellCount = rows * cols;
            cellSize = Common::maxItemCacheCellSize;
        }

        m_cacheGrids[item] = std::make_unique<CacheGrid>(maxCellCount, cellSize);
    }

    const qreal zoom{context->renderingContext().zoomFactor()};
    const QRect queryRegion{transformedGridViewport.intersected(transformer.worldToGrid(boundingBox.translated(-boundingBox.topLeft()))).toRect()};
    auto visibleCells{m_cacheGrids[item]->queryCells(queryRegion)};
    for (const auto &cell : visibleCells) {
        if (cell->dirty()) {
            cell->paint([&](QPainter &painter) -> void {
                painter.scale(zoom, zoom);
                item->draw(painter, transformer.gridToWorld(transformer.worldToGrid(boundingBox.topLeft()) + cell->rect().topLeft()));
            });
            cell->setDirty(false);
        }

        painter.drawPixmap(transformer.worldToGrid(boundingBox.topLeft()) + cell->rect().topLeft() - offset, cell->pixmap());

        // painter.save();
        // QPen pen{};
        // pen.setColor(Qt::red);
        // painter.setPen(pen);
        // painter.drawRect(cell->rect().toRectF().translated(transformer.worldToGrid(boundingBox.topLeft()) - offset));
        // painter.drawText(transformer.worldToGrid(boundingBox.topLeft()) + cell->rect().topLeft() - offset, QString::asprintf("(%d, %d)", cell->point().x(),
        // cell->point().y())); painter.restore();
    }
}

void ItemCache::clear()
{
    m_cacheGrids.clear();
}
