// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "insertitemcommand.hpp"

#include <utility>

#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"

InsertItemCommand::InsertItemCommand(QVector<std::shared_ptr<Item>> items)
    : ItemCommand{std::move(items)}
{
}

void InsertItemCommand::execute(ApplicationContext *context)
{
    auto &transformer{context->spatialContext().coordinateTransformer()};
    auto &quadtree{context->spatialContext().quadtree()};
    auto &cacheGrid{context->spatialContext().cacheGrid()};

    for (auto &item : m_items) {
        QRect dirtyRegion{transformer.worldToGrid(item->boundingBox()).toRect()};
        quadtree.insertItem(item);
        cacheGrid.markDirty(dirtyRegion);
    }
}

void InsertItemCommand::undo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext().coordinateTransformer()};
    auto &quadtree{context->spatialContext().quadtree()};
    auto &cacheGrid{context->spatialContext().cacheGrid()};
    auto &selectedItems{context->selectionContext().selectedItems()};

    for (auto &item : m_items) {
        QRect dirtyRegion{transformer.worldToGrid(item->boundingBox()).toRect()};

        selectedItems.erase(item);
        quadtree.deleteItem(item);
        cacheGrid.markDirty(dirtyRegion);
    }
}
