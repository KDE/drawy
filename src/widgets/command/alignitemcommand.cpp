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
#include "data-structures/quadtree.hpp"
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
        const auto bbox{item->boundingBox()};
        m_initialPositions.append(bbox.center());

        const auto delta{[this, &fullRegion, &bbox] {
            switch (m_alignment) {
            case ItemUtils::AlignType::AlignBottom: {
                return QPointF{0, (fullRegion.bottom() - bbox.bottom())};
            }
            case ItemUtils::AlignType::AlignHorizontalCenter: {
                return QPointF{(fullRegion.center().x() - bbox.center().x()), 0};
            }
            case ItemUtils::AlignType::AlignTop: {
                return QPointF{0, (fullRegion.top() - bbox.top())};
            }
            case ItemUtils::AlignType::AlignLeft: {
                return QPointF{(fullRegion.left() - bbox.left()), 0};
            }
            case ItemUtils::AlignType::AlignRight: {
                return QPointF{(fullRegion.right() - bbox.right()), 0};
            }
            case ItemUtils::AlignType::AlignVerticalCenter: {
                return QPointF{0, (fullRegion.center().y() - bbox.center().y())};
            }
            case ItemUtils::AlignType::Unknown: {
                return QPointF{0, 0};
            }
            }

            return QPointF{0, 0};
        }()};

        m_finalPositions.append(bbox.center() + delta);
    }
}

void AlignItemCommand::redo(ApplicationContext *context)
{
    if (m_finalPositions.isEmpty()) {
        return;
    }

    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};
    auto &quadtree{context->spatialContext()->quadtree()};

    QRectF dirtyRegion;

    int index = 0;
    for (const auto &item : std::as_const(m_items)) {
        dirtyRegion |= item->boundingBox();

        quadtree.deleteItem(item, false);

        const QTransform invertedTransform{item->transformObj().inverted()};
        const QPointF localInitialPos{invertedTransform.map(m_initialPositions.at(index))};
        const QPointF localFinalPos{invertedTransform.map(m_finalPositions.at(index))};
        const QPointF localDelta{localFinalPos - localInitialPos};
        item->translate(localDelta);

        quadtree.insertItem(item, false);

        dirtyRegion |= item->boundingBox();
        index++;
    }

    cacheGrid.markDirty(transformer.worldToGrid(dirtyRegion).toAlignedRect());
}

void AlignItemCommand::undo(ApplicationContext *context)
{
    if (m_finalPositions.isEmpty()) {
        return;
    }
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};
    auto &quadtree{context->spatialContext()->quadtree()};

    QRectF dirtyRegion{};

    int index = 0;
    for (const auto &item : std::as_const(m_items)) {
        dirtyRegion |= item->boundingBox();

        quadtree.deleteItem(item, false);

        const QTransform invertedTransform{item->transformObj().inverted()};
        const QPointF localInitialPos{invertedTransform.map(m_finalPositions.at(index))};
        const QPointF localFinalPos{invertedTransform.map(m_initialPositions.at(index))};
        const QPointF localDelta{localFinalPos - localInitialPos};
        item->translate(localDelta);

        quadtree.insertItem(item, false);

        dirtyRegion |= item->boundingBox();
        index++;
    }

    cacheGrid.markDirty(transformer.worldToGrid(dirtyRegion).toAlignedRect());
}

QString AlignItemCommand::text() const
{
    return i18n("Align Items");
}

bool AlignItemCommand::hasChanged() const
{
    return std::ranges::any_of(m_finalPositions, [](const auto &move) {
        return !move.isNull();
    });
}
