// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "groupcommand.hpp"

#include <utility>

#include "../context/applicationcontext.hpp"
#include "../context/coordinatetransformer.hpp"
#include "../context/selectioncontext.hpp"
#include "../context/spatialcontext.hpp"
#include "../data-structures/cachegrid.hpp"
#include "../data-structures/quadtree.hpp"
#include "../item/group.hpp"
#include "commandhistory.hpp"
#include "deselectcommand.hpp"
#include "selectcommand.hpp"

GroupCommand::GroupCommand(QVector<std::shared_ptr<Item>> items) : ItemCommand{std::move(items)} {
    m_group = std::make_shared<GroupItem>();

    // sort according to z order
    ApplicationContext::instance()->spatialContext().quadtree().reorder(m_items);
}

void GroupCommand::execute(ApplicationContext *context) {
    auto &quadtree{context->spatialContext().quadtree()};
    auto &selectedItems{context->selectionContext().selectedItems()};

    for (const auto &item : m_items) {
        quadtree.deleteItem(item, false);
    }

    m_group->group(m_items);
    quadtree.insertItem(m_group);

    selectedItems.clear();
    selectedItems.insert(m_group);

    context->spatialContext().cacheGrid().markDirty(m_group->boundingBox().toRect());
}

void GroupCommand::undo(ApplicationContext *context) {
    auto &quadtree{context->spatialContext().quadtree()};
    auto &selectedItems{context->selectionContext().selectedItems()};

    quadtree.deleteItem(m_group);
    selectedItems.clear();

    for (const auto &item : m_items) {
        selectedItems.insert(item);
        quadtree.insertItem(item, false);
    }

    context->spatialContext().cacheGrid().markDirty(m_group->boundingBox().toRect());
}
