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
#include "item/item.hpp"
#include <KLocalizedString>

ZorderCommand::ZorderCommand(QList<std::shared_ptr<Item>> items, ItemUtils::ZorderMove move)
    : ItemCommand{std::move(items)}
    , m_zordermove(move)
{
}

void ZorderCommand::execute(ApplicationContext *context)
{
    auto &quadtree{context->spatialContext()->quadtree()};

    QRectF dirtyRegion;
    for (const auto &item : std::as_const(m_items)) {
        m_orderIndex[item] = quadtree.zIndex(item);
        dirtyRegion |= item->boundingBox();
        quadtree.changeZorder(m_zordermove, item);
    }

    const QRect gridDirtyRegion{context->spatialContext()->coordinateTransformer().worldToGrid(dirtyRegion).toRect()};
    context->renderingContext()->cacheGrid().markDirty(gridDirtyRegion);
}

void ZorderCommand::undo([[maybe_unused]] ApplicationContext *context)
{
#if 0
    auto &quadtree{context->spatialContext()->quadtree()};

    for (const auto &item : std::as_const(m_items)) {
        for (const auto &item : std::as_const(m_items)) {
            quadtree.(m_zordermove[item], item);
        }
        // TODO
    }
#endif
}

QString ZorderCommand::commandTitle() const
{
    return i18n("Change ZOrder");
}
