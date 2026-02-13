// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "itemcache.hpp"
#include "canvas/canvas.hpp"
#include "common/constants.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "drawy_debug.h"
#include "item/item.hpp"
#include <QtMath>
#include <qloggingcategory.h>

ItemCache::ItemCache(ApplicationContext *context)
    : mApplicationContext(context)
{
}

void ItemCache::drawCached(QPainter &painter, const std::shared_ptr<Item> &item, const QRectF &queryRegion, const QPointF &offset)
{
    Q_ASSERT(item->needsCaching());

    auto &transformer{mApplicationContext->spatialContext()->coordinateTransformer()};
    auto canvas{mApplicationContext->renderingContext()->canvas()};

    const QRectF boundingBox{item->normalizedBoundingBox()};
    const QTransform transform{item->transformObj()};

    bool invertible = false;
    const QTransform inverseTransform{transform.inverted(&invertible)};
    if (!invertible) {
        qCWarning(DRAWY_LOG) << "The transform applied is not invertible. This is most likely a bug. Please report it.";
        return;
    }

    const QRectF transformedQueryRegion{transformer.worldToGrid(inverseTransform.mapRect(queryRegion))};

    auto gridIter{m_cacheGrids.find(item)};
    const bool isCached{(gridIter != m_cacheGrids.end())};

    if (item->isDirty() || !isCached) {
        const QSizeF itemSize{transformer.worldToGrid(boundingBox.size())};
        const QSizeF maxCellSize{Common::maxItemCacheCellSize.toSizeF()};

        const QPointF worldOffset{mApplicationContext->spatialContext()->offsetPos()};
        const QRectF worldViewport(worldOffset, transformer.viewToWorld(canvas->dimensions()));
        const QRectF gridViewport{transformer.worldToGrid(worldViewport)};

        const int rows{static_cast<int>(qCeil(gridViewport.width() / maxCellSize.width())) + 1};
        const int cols{static_cast<int>(qCeil(gridViewport.height() / maxCellSize.height())) + 1};
        const int maxCellCount{rows * cols};

        const QSize cellSize{itemSize.toSize().boundedTo(Common::maxItemCacheCellSize).expandedTo(QSize(1, 1))};

        auto newGrid{std::make_unique<CacheGrid>(maxCellCount * Common::itemCacheMultiplier, cellSize)};
        newGrid->setBounds(transformer.worldToGrid(boundingBox).toAlignedRect());

        if (isCached) {
            gridIter->second = std::move(newGrid);
        } else {
            gridIter = m_cacheGrids.emplace(item, std::move(newGrid)).first;
        }
        item->setDirty(false);
    }

    const qreal zoom{mApplicationContext->renderingContext()->zoomFactor()};
    auto visibleCells{gridIter->second->queryCells(transformedQueryRegion.toAlignedRect())};

    painter.save();

    const qreal transX{transform.m31() * zoom - offset.x()};
    const qreal transY{transform.m32() * zoom - offset.y()};
    painter.translate(transX, transY);

    const double angleRad{std::atan2(inverseTransform.m21(), inverseTransform.m11())};
    const double angleDeg{qRadiansToDegrees(angleRad)};
    painter.rotate(angleDeg);

    for (const auto &cell : visibleCells) {
        if (cell->dirty()) {
            cell->paint([&](QPainter &p) -> void {
                p.scale(zoom, zoom);
                item->draw(p, transformer.gridToWorld(cell->rect().topLeft().toPointF()));
            });
            cell->setDirty(false);
        }

        painter.drawPixmap(cell->rect().topLeft(), cell->pixmap());

        // Debug visualization
        /*
        painter.save();
        painter.setPen(Qt::red);
        painter.drawRect(cell->rect());
        painter.drawText(cell->rect().topLeft(),
                         QStringLiteral("(%1, %2)").arg(cell->point().x()).arg(cell->point().y()));
        painter.restore();
        */
    }
    painter.restore();
}

void ItemCache::clearItemCache(const std::shared_ptr<Item> &item)
{
    if (m_cacheGrids.erase(item) > 0) {
        qCDebug(DRAWY_LOG) << "DELETING CACHE";
    }
}

void ItemCache::clear()
{
    m_cacheGrids.clear();
}
