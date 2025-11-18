/*
* Drawy - A simple brainstorming tool with an infinite canvas
* Copyright (C) 2025 - Prayag Jain <prayagjain2@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "groupcommand.h"

#include "commandhistory.h"
#include "selectcommand.h"
#include "deselectcommand.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../context/selectioncontext.h"
#include "../context/spatialcontext.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../item/group.h"

GroupCommand::GroupCommand(QVector<std::shared_ptr<Item>> items) : ItemCommand{items} {
    m_group = std::make_shared<GroupItem>();
}

GroupCommand::~GroupCommand() {
}

void GroupCommand::execute(ApplicationContext *context) {
    auto &quadtree{context->spatialContext().quadtree()};
    auto &selectedItems{context->selectionContext().selectedItems()};
    
    std::unordered_set<std::shared_ptr<Item>> itemSet;
    for (const auto item : m_items) {
        itemSet.insert(item);
        selectedItems.erase(item);

        quadtree.deleteItem(item);
    }

    m_group->group(itemSet);
    quadtree.insertItem(m_group);

    selectedItems.insert(m_group);

    context->spatialContext().cacheGrid().markDirty(m_group->boundingBox().toRect());
}

void GroupCommand::undo(ApplicationContext *context) {
    auto &quadtree{context->spatialContext().quadtree()};
    auto &selectedItems{context->selectionContext().selectedItems()};

    quadtree.deleteItem(m_group);

    for (const auto item : m_items) {
        selectedItems.insert(item);
        quadtree.insertItem(item);
    }

    // selectedItems.insert(m_items.begin(), m_items.end());
    selectedItems.erase(m_group);

    context->spatialContext().cacheGrid().markDirty(m_group->boundingBox().toRect());
}
