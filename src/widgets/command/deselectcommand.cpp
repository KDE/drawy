// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "deselectcommand.hpp"

#include <utility>

#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "item/item.hpp"

DeselectCommand::DeselectCommand(QList<std::shared_ptr<Item>> items)
    : ItemCommand{std::move(items)}
{
}

void DeselectCommand::execute(ApplicationContext *context)
{
    auto &selectedItems{context->selectionContext()->selectedItems()};

    QRectF dirtyRegion;
    for (const auto &item : std::as_const(m_items)) {
        dirtyRegion |= item->boundingBox();
        selectedItems.erase(item);
    }

    context->renderingContext()->cacheGrid().markDirty(dirtyRegion.toRect());
}

void DeselectCommand::undo(ApplicationContext *context)
{
    auto &selectedItems{context->selectionContext()->selectedItems()};

    QRectF dirtyRegion;
    for (const auto &item : std::as_const(m_items)) {
        dirtyRegion |= item->boundingBox();
        selectedItems.insert(item);
    }

    context->renderingContext()->cacheGrid().markDirty(dirtyRegion.toRect());
}

QString DeselectCommand::commandTitle() const
{
    return QObject::tr("Deselect Object");
}
