// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "selectiontoolmovestate.hpp"

#include <memory>

#include "../../canvas/canvas.hpp"
#include "../../command/commandhistory.hpp"
#include "../../command/moveitemcommand.hpp"
#include "../../context/applicationcontext.hpp"
#include "../../context/coordinatetransformer.hpp"
#include "../../context/renderingcontext.hpp"
#include "../../context/selectioncontext.hpp"
#include "../../context/spatialcontext.hpp"
#include "../../context/uicontext.hpp"
#include "../../data-structures/cachegrid.hpp"
#include "../../data-structures/quadtree.hpp"
#include "../../event/event.hpp"
#include "../../item/item.hpp"

bool SelectionToolMoveState::mousePressed(ApplicationContext *context)
{
    auto &uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton) {
        auto &renderingContext{context->renderingContext()};
        renderingContext.canvas().setCursor(Qt::ClosedHandCursor);

        m_lastPos = uiContext.event().pos();
        m_initialPos = m_lastPos;
        m_isActive = true;
    }

    return true;
}

void SelectionToolMoveState::mouseMoved(ApplicationContext *context)
{
    auto &renderingContext{context->renderingContext()};

    if (!m_isActive) {
        renderingContext.canvas().setCursor(Qt::OpenHandCursor);
        return;
    }

    auto &spatialContext{context->spatialContext()};
    auto &selectionContext{context->selectionContext()};
    auto &transformer{spatialContext.coordinateTransformer()};

    auto &selectedItems{selectionContext.selectedItems()};

    QPointF curPos{context->uiContext().event().pos()};

    QPointF worldCurPos{transformer.viewToWorld(curPos)};
    QPointF worldLastPos{transformer.viewToWorld(m_lastPos)};
    QPointF delta{worldCurPos - worldLastPos};

    for (const auto &item : selectedItems) {
        QRectF oldBoundingBox{item->boundingBox()};

        spatialContext.cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        item->translate(delta);
        spatialContext.cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());

        spatialContext.quadtree().updateItem(item, oldBoundingBox);
    }

    m_lastPos = curPos;
    renderingContext.markForRender();
    renderingContext.markForUpdate();
}

bool SelectionToolMoveState::mouseReleased(ApplicationContext *context)
{
    auto &renderingContext{context->renderingContext()};
    auto &spatialContext{context->spatialContext()};
    auto &transformer{spatialContext.coordinateTransformer()};

    renderingContext.canvas().setCursor(Qt::OpenHandCursor);
    CommandHistory &commandHistory{spatialContext.commandHistory()};

    QPointF curPos{context->uiContext().event().pos()};
    QPointF worldOriginalPos{transformer.viewToWorld(m_initialPos)};
    QPointF worldFinalPos{transformer.viewToWorld(curPos)};
    QPointF delta{worldFinalPos - worldOriginalPos};

    if (!m_isActive)
        return false;

    m_isActive = false;

    if (delta != QPointF{0, 0}) {
        auto &selectedItems{context->selectionContext().selectedItems()};
        QVector<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};

        // TODO: Instead of un-doing the translation so that the command can execute it again,
        //       just make it not translate manually at all in the mouseMoved method
        for (auto &item : items)
            item->translate(-delta);

        commandHistory.insert(std::make_shared<MoveItemCommand>(items, delta));
    }

    return false;
}
