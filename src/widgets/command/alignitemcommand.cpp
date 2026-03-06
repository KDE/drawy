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
#include "drawy_command_debug.h"
#include "item/item.hpp"
#include <KLocalizedString>

AlignItemCommand::AlignItemCommand(QList<std::shared_ptr<Item>> items, ItemUtils::AlignType alignment)
    : ItemCommand{std::move(items)}
    , m_alignment(alignment)
{
    qCDebug(DRAWY_COMMAND_LOG) << "AlignItemCommand" << m_items.count();
    calculateMoveItems();
}

void AlignItemCommand::calculateMoveItems()
{
    if (m_alignment == ItemUtils::AlignType::Unknown) {
        return;
    }
    QRectF fullRegion;
    for (const auto &item : std::as_const(m_items)) {
        fullRegion |= item->boundingBox();
    }
    for (const auto &item : std::as_const(m_items)) {
        switch (m_alignment) {
        case ItemUtils::AlignType::AlignBottom: {
            const QPointF move{0, (fullRegion.bottom() - item->boundingBox().bottom())};
            m_moveToPoint.append(move);
            break;
        }
        case ItemUtils::AlignType::AlignHorizontalCenter: {
            const QPointF move{(fullRegion.center().x() - item->boundingBox().center().x()), 0};
            m_moveToPoint.append(move);
            break;
        }
        case ItemUtils::AlignType::AlignTop: {
            const QPointF move{0, (fullRegion.top() - item->boundingBox().top())};
            m_moveToPoint.append(move);
            break;
        }
        case ItemUtils::AlignType::AlignLeft: {
            const QPointF move{(fullRegion.left() - item->boundingBox().left()), 0};
            m_moveToPoint.append(move);
            break;
        }
        case ItemUtils::AlignType::AlignRight: {
            const QPointF move{(fullRegion.right() - item->boundingBox().right()), 0};
            m_moveToPoint.append(move);
            break;
        }
        case ItemUtils::AlignType::AlignVerticalCenter: {
            const QPointF move{0, (fullRegion.center().y() - item->boundingBox().center().y())};
            m_moveToPoint.append(move);
            break;
        }
        case ItemUtils::AlignType::Unknown:
            qCWarning(DRAWY_COMMAND_LOG) << "Invalid alignment";
            break;
        }
    }
}

void AlignItemCommand::redo(ApplicationContext *context)
{
    if (m_moveToPoint.isEmpty()) {
        return;
    }
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    QRectF dirtyRegion;
    for (const auto &item : std::as_const(m_items)) {
        dirtyRegion |= item->boundingBox();
    }
    int index = 0;
    for (const auto &item : std::as_const(m_items)) {
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        item->translate(m_moveToPoint.at(index));
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        index++;
    }
}

void AlignItemCommand::undo(ApplicationContext *context)
{
    if (m_moveToPoint.isEmpty()) {
        return;
    }
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    int index = 0;
    for (const auto &item : std::as_const(m_items)) {
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        item->translate(-m_moveToPoint.at(index));
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        index++;
    }
}

QString AlignItemCommand::text() const
{
    return i18n("Align Items");
}

bool AlignItemCommand::hasChanged() const
{
    for (const auto &move : m_moveToPoint) {
        if (!move.isNull()) {
            return true;
        }
    }
    return false;
}
