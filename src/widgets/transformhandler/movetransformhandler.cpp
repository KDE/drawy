#include "movetransformhandler.hpp"
#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/moveitemcommand.hpp"
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
            QPen pen{Common::selectionBorderColor};
            pen.setStyle(Qt::DotLine);
            painter.setPen(pen);

            for (const auto &item : std::as_const(selectedItems)) {
                painter.drawPolygon(transformer.worldToView(item->displayBoundingBox()));
            }

            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);

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

        // TODO: Instead of un-doing the translation so that the command can execute
        // it again,
        //       just make it not translate manually at all in the mouseMoved method
        for (auto &item : items) {
            if (!item->locked()) {
                const QTransform invertedTransform{item->transformObj().inverted()};
                const QPointF localInitialPos{invertedTransform.map(worldOriginalPos)};
                const QPointF localFinalPos{invertedTransform.map(worldFinalPos)};
                const QPointF localDelta{localFinalPos - localInitialPos};

                item->translate(-localDelta);
            }
        }

        commandHistory->insert(std::make_shared<MoveItemCommand>(items, worldOriginalPos, worldFinalPos));
    }

    return TransformHandler::State::Unlocked;
}
