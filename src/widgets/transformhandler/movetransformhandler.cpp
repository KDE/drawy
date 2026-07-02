// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "movetransformhandler.hpp"
#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/deselectcommand.hpp"
#include "command/moveitemcommand.hpp"
#include "command/selectcommand.hpp"
#include "common/constants.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "event/event.hpp"
#include "item/item.hpp"
#include <QPainter>
#include <QRectF>

void MoveTransformHandler::renderHandles(ApplicationContext *context)
{
    const auto &selectedItems{context->selectionContext()->selectedItems()};

    if (selectedItems.empty()) {
        return;
    }

    auto &transformer{context->spatialContext()->coordinateTransformer()};

    context->renderingContext()->canvas()->paintCanvas([&selectedItems, &transformer, &context](QPainter &painter) -> void {
        painter.setPen(Common::selectionBorderPen());

        if (selectedItems.size() == 1) {
            const auto &item{(*selectedItems.begin())};
            painter.drawPolygon(transformer.worldToView(item->displayBoundingBox()));
        } else {
            const QPen pen{Common::selectionBorderColor};
            painter.setPen(pen);

            for (const auto &item : std::as_const(selectedItems)) {
                painter.drawPolygon(transformer.worldToView(item->displayBoundingBox()));
            }

            painter.drawPolygon(transformer.worldToView(context->selectionContext()->selectionBox()));
        }
    });

    context->renderingContext()->markForUpdate();
}

bool MoveTransformHandler::shouldActivate(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};

    const auto [selectionBox, selectionBoxTransform]{context->selectionContext()->selectionBoxWithTransform()};
    const QPointF worldPos{transformer.viewToWorld(context->uiContext()->appEvent()->pos())};
    const QPointF relativeCurPos{selectionBoxTransform.inverted().map(worldPos)};

    return selectionBox.contains(relativeCurPos);
}

TransformHandler::State MoveTransformHandler::mousePressed(ApplicationContext *context)
{
    auto uiContext{context->uiContext()};

    if (uiContext->appEvent()->button() == Qt::LeftButton) {
        auto renderingContext{context->renderingContext()};

        renderingContext->canvas()->setCursor(Qt::ArrowCursor);

        m_lastPos = uiContext->appEvent()->pos();
        m_initialPos = m_lastPos;
        m_isActive = true;
    }

    return TransformHandler::State::Locked;
}

TransformHandler::State MoveTransformHandler::mouseMoved(ApplicationContext *context)
{
    auto renderingContext{context->renderingContext()};

    if (!m_isActive) {
        renderingContext->canvas()->setCursor(Qt::ArrowCursor);
        return TransformHandler::State::Unlocked;
    }

    auto spatialContext{context->spatialContext()};
    auto selectionContext{context->selectionContext()};
    auto transformer{spatialContext->coordinateTransformer()};

    const auto &selectedItems{selectionContext->selectedItems()};

    const QPointF curPos{context->uiContext()->appEvent()->pos()};

    const QPointF worldCurPos{transformer.viewToWorld(curPos)};
    const QPointF worldLastPos{transformer.viewToWorld(m_lastPos)};

    QRect dirtyRegion{};
    for (const auto &item : selectedItems) {
        if (!item->locked()) {
            const QTransform invertedTransform{item->transformObj().inverted()};
            const QPointF localCurPos{invertedTransform.map(worldCurPos)};
            const QPointF localLastPos{invertedTransform.map(worldLastPos)};
            const QPointF localDelta{localCurPos - localLastPos};

            spatialContext->quadtree().deleteItem(item, false);
            dirtyRegion |= transformer.worldToGrid(item->boundingBox()).toRect();

            item->translate(localDelta);

            spatialContext->quadtree().insertItem(item, false);
            dirtyRegion |= transformer.worldToGrid(item->boundingBox()).toRect();
        }
    }

    renderingContext->cacheGrid().markDirty(dirtyRegion);

    m_lastPos = curPos;
    renderingContext->markForRender();
    renderingContext->markForUpdate();

    return TransformHandler::State::Locked;
}

TransformHandler::State MoveTransformHandler::mouseReleased(ApplicationContext *context)
{
    auto renderingContext{context->renderingContext()};
    auto spatialContext{context->spatialContext()};
    auto transformer{spatialContext->coordinateTransformer()};

    renderingContext->canvas()->setCursor(Qt::ArrowCursor);
    auto commandHistory{spatialContext->commandHistory()};

    const QPointF curPos{context->uiContext()->appEvent()->pos()};
    const QPointF worldOriginalPos{transformer.viewToWorld(m_initialPos)};
    const QPointF worldFinalPos{transformer.viewToWorld(curPos)};
    const QPointF delta{worldFinalPos - worldOriginalPos};

    if (!m_isActive) {
        return TransformHandler::State::Unlocked;
    }

    m_isActive = false;

    if (delta != QPointF{0, 0}) {
        auto &selectedItems{context->selectionContext()->selectedItems()};
        const QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};

        for (auto &item : items) {
            if (!item->locked()) {
                const QTransform invertedTransform{item->transformObj().inverted()};
                const QPointF localInitialPos{invertedTransform.map(worldOriginalPos)};
                const QPointF localFinalPos{invertedTransform.map(worldFinalPos)};
                const QPointF localDelta{localFinalPos - localInitialPos};

                item->translate(-localDelta);
            }
        }

        commandHistory->push(std::make_shared<MoveItemCommand>(items, worldOriginalPos, worldFinalPos));
    } else {
        auto uiContext{context->uiContext()};

        const QRectF cursorRegion{createHandle(transformer.viewToWorld(uiContext->appEvent()->pos()), Common::selectionCursorHitSize)};
        const auto intersectingItems{spatialContext->quadtree().queryItems(cursorRegion, [](const std::shared_ptr<Item> &item, auto &region) {
            return item->intersects(region);
        })};

        const auto selectedItems{context->selectionContext()->selectedItems()};
        const QList<std::shared_ptr<Item>> selectedItemsList{selectedItems.begin(), selectedItems.end()};

        if (intersectingItems.empty()) {
            // if the users clicks in an empty region, deselect everything
            commandHistory->push(std::make_shared<DeselectCommand>(selectedItemsList));

        } else if (uiContext->appEvent()->modifiers().testFlag(Qt::ShiftModifier)) {
            // if the users holds shift and clicks on an element, deselect or select it, depending on the situation

            if (selectedItems.contains(intersectingItems.back())) {
                commandHistory->push(std::make_shared<DeselectCommand>(QList<std::shared_ptr<Item>>{intersectingItems.back()}));
            } else {
                commandHistory->push(std::make_shared<SelectCommand>(QList<std::shared_ptr<Item>>{intersectingItems.back()}));
            }
        } else {
            // deselect everything and select that one item
            commandHistory->push(std::make_shared<DeselectCommand>(selectedItemsList));
            commandHistory->push(std::make_shared<SelectCommand>(QList<std::shared_ptr<Item>>{intersectingItems.back()}));
        }

        renderingContext->markForRender();
        renderingContext->markForUpdate();
    }

    return TransformHandler::State::Unlocked;
}
