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
#include "drawy_command_debug.h"
#include "item/item.hpp"
#include <KLocalizedString>

MoveItemCommand::MoveItemCommand(QList<std::shared_ptr<Item>> items, const QPointF worldInitialPos, const QPointF worldFinalPos)
    : ItemCommand{std::move(items)}
    , m_worldInitialPos{worldInitialPos}
    , m_worldFinalPos{worldFinalPos}
{
    qCDebug(DRAWY_COMMAND_LOG) << "MoveItemCommand" << m_items.count();
}

void MoveItemCommand::redo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    QRectF dirtyRegion{};
    for (const auto &item : std::as_const(m_items)) {
        dirtyRegion |= item->boundingBox();

        const QTransform invertedTransform{item->transformObj().inverted()};
        const QPointF localFinalPos{invertedTransform.map(m_worldFinalPos)};
        const QPointF localInitialPos{invertedTransform.map(m_worldInitialPos)};
        const QPointF localDelta{localFinalPos - localInitialPos};

        item->translate(localDelta);

        dirtyRegion |= item->boundingBox();
    }

    cacheGrid.markDirty(transformer.worldToGrid(dirtyRegion).toRect());
}

void MoveItemCommand::undo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    QRectF dirtyRegion{};
    for (const auto &item : std::as_const(m_items)) {
        dirtyRegion |= item->boundingBox();

        const QTransform invertedTransform{item->transformObj().inverted()};
        const QPointF localFinalPos{invertedTransform.map(m_worldFinalPos)};
        const QPointF localInitialPos{invertedTransform.map(m_worldInitialPos)};
        const QPointF localDelta{localFinalPos - localInitialPos};

        item->translate(-localDelta);

        dirtyRegion |= item->boundingBox();
    }

    cacheGrid.markDirty(transformer.worldToGrid(dirtyRegion).toRect());
}

QString MoveItemCommand::text() const
{
    return i18n("Move Object");
}
