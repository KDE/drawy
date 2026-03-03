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
    for (const auto &item : m_items) {
        if (item->formType() == Item::FormType::Group) {
            m_groups.push_back(std::dynamic_pointer_cast<GroupItem>(item));
        }
    }
}

void UngroupCommand::execute(ApplicationContext *context)
{
    auto &quadtree{context->spatialContext()->quadtree()};

    context->selectionContext()->reset();

    QRectF dirtyRegion;
    for (const auto &group : std::as_const(m_groups)) {
        quadtree.deleteItem(group);

        dirtyRegion |= group->boundingBox();

        auto subItems{group->unGroup()};
        for (const auto &subItem : subItems) {
            quadtree.insertItem(subItem, false);
            context->selectionContext()->addToSelection(subItem);
        }
    }

    context->renderingContext()->cacheGrid().markDirty(dirtyRegion.toRect());
}

void UngroupCommand::undo(ApplicationContext *context)
{
    auto &quadtree{context->spatialContext()->quadtree()};

    context->selectionContext()->reset();

    QRectF dirtyRegion;
    for (const auto &group : std::as_const(m_groups)) {
        quadtree.insertItem(group);
        context->selectionContext()->addToSelection(group);
        dirtyRegion |= group->boundingBox();

        const auto subItems{group->unGroup()};
        for (const auto &subItem : subItems) {
            quadtree.deleteItem(subItem, false);
        }
    }

    context->renderingContext()->cacheGrid().markDirty(dirtyRegion.toRect());
}

QString UngroupCommand::commandTitle() const
{
    return i18n("Ungroup Objects");
}
