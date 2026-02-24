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

void MoveItemCommand::execute(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    for (const auto &item : std::as_const(m_items)) {
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());

        const QTransform invertedTransform{item->transformObj().inverted()};
        const QPointF localFinalPos{invertedTransform.map(m_worldFinalPos)};
        const QPointF localInitialPos{invertedTransform.map(m_worldInitialPos)};
        const QPointF localDelta{localFinalPos - localInitialPos};

        item->translate(localDelta);

        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
    }
}

void MoveItemCommand::undo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    for (const auto &item : std::as_const(m_items)) {
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());

        const QTransform invertedTransform{item->transformObj().inverted()};
        const QPointF localFinalPos{invertedTransform.map(m_worldFinalPos)};
        const QPointF localInitialPos{invertedTransform.map(m_worldInitialPos)};
        const QPointF localDelta{localFinalPos - localInitialPos};

        item->translate(-localDelta);

        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
    }
}

QString MoveItemCommand::commandTitle() const
{
    return i18n("Move Object");
}
