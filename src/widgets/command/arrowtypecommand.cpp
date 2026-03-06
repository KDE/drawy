/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "arrowtypecommand.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "drawy_command_debug.h"
#include "item/arrow.hpp"
#include "item/item.hpp"
#include <KLocalizedString>
#include <QRectF>

ArrowTypeCommand::ArrowTypeCommand(QList<std::shared_ptr<Item>> items, ArrowUtils::ArrowPos arrowPos, ArrowUtils::ArrowType arrowStyle)
    : ItemCommand{std::move(items)}
    , m_arrowPos(arrowPos)
    , m_arrowStyle(arrowStyle)
{
    qCDebug(DRAWY_COMMAND_LOG) << "ArrowTypeCommand" << m_items.count();
}

void ArrowTypeCommand::redo(ApplicationContext *context)
{
    QRectF dirtyRegion;
    for (const auto &item : std::as_const(m_items)) {
        if (auto arrowItem = dynamic_cast<ArrowItem *>(item.get())) {
            if (m_arrowPos == ArrowUtils::ArrowPos::EndArrow) {
                m_originalArrowStyle[item] = arrowItem->endArrow();
                arrowItem->setEndArrow(m_arrowStyle);
            } else if (m_arrowPos == ArrowUtils::ArrowPos::StartArrow) {
                m_originalArrowStyle[item] = arrowItem->startArrow();
                arrowItem->setStartArrow(m_arrowStyle);
            }
            dirtyRegion |= item->boundingBox();
        }
    }

    const QRect gridDirtyRegion{context->spatialContext()->coordinateTransformer().worldToGrid(dirtyRegion).toRect()};
    context->renderingContext()->cacheGrid().markDirty(gridDirtyRegion);
}

void ArrowTypeCommand::undo(ApplicationContext *context)
{
    QRectF dirtyRegion;
    for (const auto &item : std::as_const(m_items)) {
        if (auto arrowItem = dynamic_cast<ArrowItem *>(item.get())) {
            dirtyRegion |= item->boundingBox();
            if (m_arrowPos == ArrowUtils::ArrowPos::EndArrow) {
                arrowItem->setEndArrow(m_originalArrowStyle[item]);
            } else if (m_arrowPos == ArrowUtils::ArrowPos::StartArrow) {
                arrowItem->setStartArrow(m_originalArrowStyle[item]);
            }
        }
    }
    const QRect gridDirtyRegion{context->spatialContext()->coordinateTransformer().worldToGrid(dirtyRegion).toRect()};
    context->renderingContext()->cacheGrid().markDirty(gridDirtyRegion);
}

QString ArrowTypeCommand::text() const
{
    return i18n("Change Arrow Type");
}
