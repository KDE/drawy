// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "rotateitemcommand.hpp"

#include <utility>

#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "drawy_command_debug.h"
#include "item/item.hpp"
#include <KLocalizedString>

RotateItemCommand::RotateItemCommand(QList<std::shared_ptr<Item>> items, const qreal angle, const QPointF pivot)
    : ItemCommand{std::move(items)}
    , m_angle{angle}
    , m_pivot{pivot}
{
    qCDebug(DRAWY_COMMAND_LOG) << "RotateItemCommand" << m_items.count();
}

void RotateItemCommand::redo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    for (const auto &item : std::as_const(m_items)) {
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());

        item->rotate(m_angle, item->transformObj().inverted().map(m_pivot));
        item->commitTransformation();

        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
    }
}

void RotateItemCommand::undo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    for (const auto &item : std::as_const(m_items)) {
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());

        item->rotate(-m_angle, item->transformObj().inverted().map(m_pivot));
        item->commitTransformation();

        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
    }
}

QString RotateItemCommand::text() const
{
    return i18n("Rotate Object");
}
