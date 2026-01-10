// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ungroupcommand.hpp"

#include <memory>

#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "item/group.hpp"

UngroupCommand::UngroupCommand(const QList<std::shared_ptr<Item>> &items)
    : ItemCommand{items}
{
    for (const auto &item : items) {
        if (item->type() == Item::Type::Group) {
            m_groups.push_back(std::dynamic_pointer_cast<GroupItem>(item));
        }
    }
}

void UngroupCommand::execute(ApplicationContext *context)
{
    auto &quadtree{context->spatialContext().quadtree()};
    auto &selectedItems{context->selectionContext().selectedItems()};

    selectedItems.clear();

    QRectF dirtyRegion{};
    for (const auto &group : m_groups) {
        quadtree.deleteItem(group);

        dirtyRegion |= group->boundingBox();

        auto subItems{group->unGroup()};
        for (const auto &subItem : subItems) {
            quadtree.insertItem(subItem, false);
            selectedItems.insert(subItem);
        }
    }

    context->spatialContext().cacheGrid().markDirty(dirtyRegion.toRect());
}

void UngroupCommand::undo(ApplicationContext *context)
{
    auto &quadtree{context->spatialContext().quadtree()};
    auto &selectedItems{context->selectionContext().selectedItems()};

    selectedItems.clear();

    QRectF dirtyRegion{};
    for (const auto &group : std::as_const(m_groups)) {
        quadtree.insertItem(group);
        selectedItems.insert(group);
        dirtyRegion |= group->boundingBox();

        auto subItems{group->unGroup()};
        for (const auto &subItem : subItems) {
            quadtree.deleteItem(subItem, false);
        }
    }

    context->spatialContext().cacheGrid().markDirty(dirtyRegion.toRect());
}
