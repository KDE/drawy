/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "alignitemcommand.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "item/item.hpp"

AlignItemCommand::AlignItemCommand(QList<std::shared_ptr<Item>> items, AlignItemCommand::Alignment alignment)
    : ItemCommand{std::move(items)}
    , m_alignment(alignment)
{
}

void AlignItemCommand::execute(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    QRectF dirtyRegion;
    for (const auto &item : std::as_const(m_items)) {
        dirtyRegion |= item->boundingBox();
    }
    for (const auto &item : std::as_const(m_items)) {
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
    }
}

void AlignItemCommand::undo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    for (const auto &item : std::as_const(m_items)) {
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
    }
}

QString AlignItemCommand::commandTitle() const
{
    return QObject::tr("Align Items");
}
