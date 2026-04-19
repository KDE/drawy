// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "resizeitemcommand.hpp"

#include <utility>

#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "drawy_command_debug.h"
#include "item/item.hpp"
#include <KLocalizedString>

ResizeItemCommand::ResizeItemCommand(QList<std::shared_ptr<Item>> items,
                                     const QTransform transform,
                                     const QTransform lockedTransform,
                                     std::unordered_map<std::shared_ptr<Item>, bool> useLocked)
    : ItemCommand{std::move(items)}
    , m_transform{transform}
    , m_lockedTransform{lockedTransform}
    , m_useLocked{std::move(useLocked)}
{
    qCDebug(DRAWY_COMMAND_LOG) << "ResizeItemCommand" << m_items.count();
}

void ResizeItemCommand::redo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    QRectF dirtyRegion{};
    for (const auto &item : std::as_const(m_items)) {
        dirtyRegion |= item->boundingBox();

        if (m_useLocked[item]) {
            item->resize(m_lockedTransform);
        } else {
            item->resize(m_transform);
        }

        item->commitTransformation();

        dirtyRegion |= item->boundingBox();
    }

    cacheGrid.markDirty(transformer.worldToGrid(dirtyRegion).toAlignedRect());
}

void ResizeItemCommand::undo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    QRectF dirtyRegion{};
    for (const auto &item : std::as_const(m_items)) {
        dirtyRegion |= item->boundingBox();

        if (m_useLocked[item]) {
            item->resize(m_lockedTransform.inverted());
        } else {
            item->resize(m_transform.inverted());
        }

        item->commitTransformation();

        dirtyRegion |= item->boundingBox();
    }

    cacheGrid.markDirty(transformer.worldToGrid(dirtyRegion).toAlignedRect());
}

QString ResizeItemCommand::text() const
{
    return i18n("Resize Object");
}
