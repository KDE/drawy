// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "selectcommand.hpp"

#include <utility>

#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "drawy_command_debug.h"
#include "item/item.hpp"
#include <KLocalizedString>

SelectCommand::SelectCommand(QList<std::shared_ptr<Item>> items)
    : ItemCommand{std::move(items)}
{
    qCDebug(DRAWY_COMMAND_LOG) << "SelectCommand" << m_items.count();
}

void SelectCommand::execute(ApplicationContext *context)
{
    QRectF dirtyRegion;
    for (const auto &item : std::as_const(m_items)) {
        dirtyRegion |= item->boundingBox();
    }

    context->selectionContext()->addToSelection(m_items.begin(), m_items.end());
    context->renderingContext()->cacheGrid().markDirty(dirtyRegion.toRect());
}

void SelectCommand::undo(ApplicationContext *context)
{
    QRectF dirtyRegion;
    for (const auto &item : std::as_const(m_items)) {
        dirtyRegion |= item->boundingBox();
    }

    context->selectionContext()->removeFromSelection(m_items.begin(), m_items.end());
    context->renderingContext()->cacheGrid().markDirty(dirtyRegion.toRect());
}

QString SelectCommand::commandTitle() const
{
    return i18n("Select Object");
}
