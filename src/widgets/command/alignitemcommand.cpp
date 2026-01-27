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

AlignItemCommand::AlignItemCommand(QList<std::shared_ptr<Item>> items, AlignItemCommand::Alignment alignment)
    : ItemCommand{std::move(items)}
    , m_alignment(alignment)
{
    calculateMoveItems();
}

void AlignItemCommand::calculateMoveItems()
{
    if (m_alignment == Alignment::Unknow) {
        return;
    }
    QRectF fullRegion;
    for (const auto &item : std::as_const(m_items)) {
        fullRegion |= item->boundingBox();
    }
    for (const auto &item : std::as_const(m_items)) {
        switch (m_alignment) {
        case Alignment::AlignToBottom: {
            const QPointF move{0, (fullRegion.bottom() - item->boundingBox().bottom())};
            m_moveToPoint.append(move);
            break;
        }
        case Alignment::AlignHorizontalCenter:
            break;
        case Alignment::AlignToTop: {
            const QPointF move{0, (fullRegion.top() - item->boundingBox().top())};
            m_moveToPoint.append(move);
            break;
        }
        case Alignment::AlignToLeft: {
            const QPointF move{(fullRegion.left() - item->boundingBox().left()), 0};
            m_moveToPoint.append(move);
            break;
        }
        case Alignment::AlignToRight: {
            const QPointF move{(fullRegion.right() - item->boundingBox().right()), 0};
            m_moveToPoint.append(move);
            break;
        }
        case Alignment::AlignVerticalCenter:
            break;
        case Alignment::Unknow:
            qCWarning(DRAWY_COMMAND_LOG) << "Invalid alignment";
            break;
        }
    }
}

void AlignItemCommand::execute(ApplicationContext *context)
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

QString AlignItemCommand::commandTitle() const
{
    return QObject::tr("Align Items");
}
