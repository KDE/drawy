// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "groupcommand.hpp"

#include <utility>

#include "commandhistory.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "deselectcommand.hpp"
#include "item/group.hpp"
#include "selectcommand.hpp"

GroupCommand::GroupCommand(ApplicationContext *context, QList<std::shared_ptr<Item>> items)
    : ItemCommand{std::move(items)}
    , m_group(std::make_shared<GroupItem>())
    , m_context(context)
{
    // sort according to z order
    m_context->spatialContext()->quadtree().reorder(m_items);
}

void GroupCommand::execute(ApplicationContext *context)
{
    auto &quadtree{context->spatialContext()->quadtree()};

    for (const auto &item : std::as_const(m_items)) {
        quadtree.deleteItem(item, false);
    }

    m_group->group(m_items);
    quadtree.insertItem(m_group);

    context->selectionContext()->reset();
    context->selectionContext()->addToSelection(m_group);

    context->renderingContext()->cacheGrid().markDirty(m_group->boundingBox().toRect());
}

void GroupCommand::undo(ApplicationContext *context)
{
    auto &quadtree{context->spatialContext()->quadtree()};

    quadtree.deleteItem(m_group);
    context->selectionContext()->reset();

    for (const auto &item : std::as_const(m_items)) {
        quadtree.insertItem(item, false);
    }

    context->selectionContext()->addToSelection(m_items.begin(), m_items.end());
    context->renderingContext()->cacheGrid().markDirty(m_group->boundingBox().toRect());
}

QString GroupCommand::commandTitle() const
{
    return QObject::tr("Group Object");
}
