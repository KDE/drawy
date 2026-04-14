// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ungroupcommand.hpp"

#include <memory>

#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "drawy_command_debug.h"
#include "item/group.hpp"
#include <KLocalizedString>

UngroupCommand::UngroupCommand(QList<std::shared_ptr<Item>> items)
    : ItemCommand{std::move(items)}
{
    qCDebug(DRAWY_COMMAND_LOG) << "UngroupCommand" << m_items.count();
    for (const auto &item : std::as_const(m_items)) {
        if (item->formType() == Item::FormType::Group) {
            m_groups.push_back(std::dynamic_pointer_cast<GroupItem>(item));
        }
    }
}

void UngroupCommand::redo(ApplicationContext *context)
{
    auto &quadtree{context->spatialContext()->quadtree()};

    context->selectionContext()->reset();

    QRectF dirtyRegion;
    for (const auto &group : std::as_const(m_groups)) {
        dirtyRegion |= group->boundingBox().toAlignedRect();
        m_ungroupedItems.push_back(group->unGroup());
        context->spatialContext()->quadtree().reorder(m_ungroupedItems.back());

        group->setTransform({});

        quadtree.deleteItem(group);
        context->selectionContext()->reset();

        for (const auto &item : std::as_const(m_ungroupedItems.back())) {
            quadtree.insertItem(item, false);
        }

        context->selectionContext()->addToSelection(m_ungroupedItems.back().begin(), m_ungroupedItems.back().end());
    }

    context->renderingContext()->cacheGrid().markDirty(dirtyRegion.toRect());
}

void UngroupCommand::undo(ApplicationContext *context)
{
    auto &quadtree{context->spatialContext()->quadtree()};

    context->selectionContext()->reset();

    const qsizetype groupCount{m_ungroupedItems.size()};
    Q_ASSERT(groupCount == m_groups.size());

    QRectF dirtyRegion;
    for (qsizetype pos{0}; pos < groupCount; pos++) {
        for (const auto &item : std::as_const(m_ungroupedItems[pos])) {
            quadtree.deleteItem(item, false);
        }

        m_groups[pos]->group(m_ungroupedItems[pos]);
        quadtree.insertItem(m_groups[pos]);

        dirtyRegion |= m_groups[pos]->boundingBox().toAlignedRect();

        context->selectionContext()->addToSelection(m_groups[pos]);
    }

    context->renderingContext()->cacheGrid().markDirty(dirtyRegion.toRect());

    m_ungroupedItems.clear();
}

QString UngroupCommand::text() const
{
    return i18n("Ungroup Objects");
}
