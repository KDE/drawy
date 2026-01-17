// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "removeitemcommand.hpp"

#include <utility>

#include "common/constants.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "item/item.hpp"
#include "item/itemcache/itemcache.hpp"

RemoveItemCommand::RemoveItemCommand(QList<std::shared_ptr<Item>> items)
    : ItemCommand{std::move(items)}
{
}

void RemoveItemCommand::execute(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &quadtree{context->spatialContext()->quadtree()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};
    auto &selectedItems{context->selectionContext()->selectedItems()};
    auto &itemCache{context->renderingContext()->itemCache()};

    for (auto &item : m_items) {
        const QRect dirtyRegion{transformer.worldToGrid(item->boundingBox()).toRect()};

        selectedItems.erase(item);
        quadtree.deleteItem(item, false);
        cacheGrid.markDirty(dirtyRegion);
        itemCache.clearItemCache(item);
    }
}

void RemoveItemCommand::undo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &quadtree{context->spatialContext()->quadtree()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    for (auto &item : m_items) {
        const QRect dirtyRegion{transformer.worldToGrid(item->boundingBox()).toRect()};

        quadtree.insertItem(item, false);
        cacheGrid.markDirty(dirtyRegion);
    }
}
