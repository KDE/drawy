/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "zordercommand.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "drawy_command_debug.h"
#include "item/item.hpp"
#include <KLocalizedString>
#include <algorithm>

namespace
{
[[nodiscard]] bool shouldExecuteInReverseOrder(const ItemUtils::ZorderMove move)
{
    return move == ItemUtils::ZorderMove::BringForward || move == ItemUtils::ZorderMove::SendToBack;
}
}

ZorderCommand::ZorderCommand(QList<std::shared_ptr<Item>> items, ItemUtils::ZorderMove move)
    : ItemCommand{std::move(items)}
    , m_zorderMove(move)
{
    qCDebug(DRAWY_COMMAND_LOG) << "ZorderCommand" << m_items.count();
}

void ZorderCommand::redo(ApplicationContext *context)
{
    auto &quadtree{context->spatialContext()->quadtree()};
    auto sortedItems = m_items;

    std::sort(sortedItems.begin(), sortedItems.end(), [&quadtree](const auto &leftItem, const auto &rightItem) {
        return quadtree.zIndex(leftItem) < quadtree.zIndex(rightItem);
    });
    m_originalSortedItems = sortedItems;

    QRectF dirtyRegion;
    m_originalOrder.clear();
    for (const auto &item : std::as_const(sortedItems)) {
        const auto position = quadtree.zorderPosition(item);
        m_originalOrder[item] = {position.previousItem, position.nextItem};
        dirtyRegion |= item->boundingBox();
    }

    if (shouldExecuteInReverseOrder(m_zorderMove)) {
        std::reverse(sortedItems.begin(), sortedItems.end());
    }

    for (const auto &item : std::as_const(sortedItems)) {
        quadtree.changeZorder(m_zorderMove, item);
    }

    const QRect gridDirtyRegion{context->spatialContext()->coordinateTransformer().worldToGrid(dirtyRegion).toRect()};
    context->renderingContext()->cacheGrid().markDirty(gridDirtyRegion);
}

void ZorderCommand::undo(ApplicationContext *context)
{
    auto &quadtree{context->spatialContext()->quadtree()};
    const auto &sortedItems = m_originalSortedItems;
    QRectF dirtyRegion;

    for (const auto &item : std::as_const(sortedItems)) {
        const auto originalState = m_originalOrder.find(item);
        if (originalState == m_originalOrder.end()) {
            continue;
        }

        const auto previousItem = originalState->second.previousItem.lock();
        const auto nextItem = originalState->second.nextItem.lock();
        dirtyRegion |= item->boundingBox();
        quadtree.restoreZorderPosition(item, {previousItem, nextItem});
    }

    const QRect gridDirtyRegion{context->spatialContext()->coordinateTransformer().worldToGrid(dirtyRegion).toRect()};
    context->renderingContext()->cacheGrid().markDirty(gridDirtyRegion);
}

QString ZorderCommand::text() const
{
    return i18n("Change ZOrder");
}
