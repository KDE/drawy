// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "moveitemcommand.hpp"

#include <utility>

#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "item/item.hpp"

MoveItemCommand::MoveItemCommand(QList<std::shared_ptr<Item>> items, QPointF delta)
    : ItemCommand{std::move(items)}
    , m_delta{delta}
{
}

void MoveItemCommand::execute(ApplicationContext *context)
{
    auto &transformer{context->spatialContext().coordinateTransformer()};
    auto &cacheGrid{context->renderingContext().cacheGrid()};

    for (auto &item : m_items) {
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        item->translate(m_delta);
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
    }
}

void MoveItemCommand::undo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext().coordinateTransformer()};
    auto &cacheGrid{context->renderingContext().cacheGrid()};

    for (auto &item : m_items) {
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        item->translate(-m_delta);
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
    }
}
