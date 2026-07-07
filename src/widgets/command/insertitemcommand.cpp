// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "insertitemcommand.hpp"

#include <utility>

#include "components/toolbar.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "drawy_command_debug.h"
#include "tools/selectiontool/selectiontool.hpp"
#include <KLocalizedString>

InsertItemCommand::InsertItemCommand(QList<std::shared_ptr<Item>> items)
    : ItemCommand{std::move(items)}
{
    qCDebug(DRAWY_COMMAND_LOG) << "InsertItemCommand" << m_items.count();
}

void InsertItemCommand::redo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &quadtree{context->spatialContext()->quadtree()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    QRectF dirtyRegion;

    for (const auto &item : std::as_const(m_items)) {
        dirtyRegion |= item->boundingBox();

        quadtree.insertItem(item);
    }

    cacheGrid.markDirty(transformer.worldToGrid(dirtyRegion).toAlignedRect());

    auto toolBar{context->uiContext()->toolBar()};
    if (toolBar->curTool().type() == Tool::Type::Selection) {
        dynamic_cast<SelectionTool &>(toolBar->curTool()).renderHighlights(context);
    }
}

void InsertItemCommand::undo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &quadtree{context->spatialContext()->quadtree()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    QRectF dirtyRegion;
    for (const auto &item : std::as_const(m_items)) {
        dirtyRegion |= item->boundingBox();

        context->selectionContext()->removeFromSelection(item);
        quadtree.deleteItem(item);
    }

    cacheGrid.markDirty(transformer.worldToGrid(dirtyRegion).toAlignedRect());

    auto toolBar{context->uiContext()->toolBar()};
    if (toolBar->curTool().type() == Tool::Type::Selection) {
        dynamic_cast<SelectionTool &>(toolBar->curTool()).renderHighlights(context);
    }
}

QString InsertItemCommand::text() const
{
    return i18n("Insert Object");
}
