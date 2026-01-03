// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "selectcommand.hpp"

#include <utility>

#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "item/item.hpp"

SelectCommand::SelectCommand(QVector<std::shared_ptr<Item>> items)
    : ItemCommand{std::move(items)}
{
}

void SelectCommand::execute(ApplicationContext *context)
{
    auto &selectedItems{context->selectionContext()->selectedItems()};

    QRectF dirtyRegion{};
    for (const auto &item : m_items) {
        dirtyRegion |= item->boundingBox();
        selectedItems.insert(item);
    }

    context->spatialContext()->cacheGrid()->markDirty(dirtyRegion.toRect());
}

void SelectCommand::undo(ApplicationContext *context)
{
    auto &selectedItems{context->selectionContext()->selectedItems()};

    QRectF dirtyRegion{};
    for (const auto &item : m_items) {
        dirtyRegion |= item->boundingBox();
        selectedItems.erase(item);
    }

    context->spatialContext()->cacheGrid()->markDirty(dirtyRegion.toRect());
}
