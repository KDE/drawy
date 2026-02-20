#include "lineresizetransformhandler.hpp"
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
#include "item/line.hpp"
#include <QPainter>
#include <QRectF>
#include <algorithm>

using namespace Qt::StringLiterals;

namespace
{
std::shared_ptr<LineItem> getSelectedLineItem(ApplicationContext *context)
{
    const auto &selectedItems{context->selectionContext()->selectedItems()};
    Q_ASSERT(selectedItems.size() == 1);
    return std::dynamic_pointer_cast<LineItem>(*selectedItems.begin());
}
}

void LineResizeTransformHandler::renderHandles(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    const auto handles{getHandles(getSelectedLineItem(context))};

    context->renderingContext()->canvas()->paintCanvas([context, &transformer, &handles](QPainter &painter) {
        painter.setPen(Common::selectionBorderPen);
        painter.setBrush(context->renderingContext()->canvas()->canvasBg());

        for (const auto &handle : handles) {
            painter.drawEllipse(createHandle(transformer.worldToView(handle.rect.center()), Common::selectionHandleSize));
        }
    });
}

bool LineResizeTransformHandler::shouldActivate(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    const auto mousePos{context->uiContext()->appEvent()->pos()};
    const auto handles{getHandles(getSelectedLineItem(context))};

    return std::ranges::any_of(handles, [&transformer, mousePos](const auto &handle) {
        return createHandle(transformer.worldToView(handle.rect.center()), Common::selectionHandleSize).contains(mousePos);
    });
}

TransformHandler::State LineResizeTransformHandler::mousePressed(ApplicationContext *context)
{
    if (context->uiContext()->appEvent()->button() == Qt::LeftButton) {
        context->renderingContext()->canvas()->setCursor(Qt::ClosedHandCursor);
        m_isActive = true;
    }

    return TransformHandler::State::Locked;
}

TransformHandler::State LineResizeTransformHandler::mouseMoved(ApplicationContext *context)
{
    auto item{getSelectedLineItem(context)};
    auto renderingContext{context->renderingContext()};
    const auto mousePos{context->uiContext()->appEvent()->pos()};
    const auto worldPos{context->spatialContext()->coordinateTransformer().viewToWorld(mousePos)};

    if (!m_isActive) {
        auto &transformer{context->spatialContext()->coordinateTransformer()};
        const auto handles{getHandles(item)};
        auto it{std::ranges::find_if(handles, [&transformer, mousePos](const auto &handle) {
            return createHandle(transformer.worldToView(handle.rect.center()), Common::selectionHandleSize).contains(mousePos);
        })};

        if (it != handles.end()) {
            m_curHandleType = it->handleType;
            renderingContext->canvas()->setCursor(Qt::OpenHandCursor);
        } else {
            m_curHandleType = ResizeHandleType::None;
        }
    } else {
        auto &quadtree{context->spatialContext()->quadtree()};
        auto &transformer{context->spatialContext()->coordinateTransformer()};

        const auto relativeCurPos{item->transformObj().inverted().map(worldPos)};

        QRectF dirtyRegion{item->boundingBox()};
        quadtree.deleteItem(item);

        switch (m_curHandleType) {
        case ResizeHandleType::Start:
            item->setStart(relativeCurPos);
            break;
        case ResizeHandleType::End:
            item->setEnd(relativeCurPos);
            break;
        default:
            break;
        }

        quadtree.insertItem(item);
        dirtyRegion |= item->boundingBox();

        renderingContext->cacheGrid().markDirty(transformer.worldToGrid(dirtyRegion.toAlignedRect()));
        renderingContext->markForRender();
        renderingContext->markForUpdate();

        return TransformHandler::State::Locked;
    }

    return TransformHandler::State::Unlocked;
}

TransformHandler::State LineResizeTransformHandler::mouseReleased(ApplicationContext *)
{
    m_isActive = false;
    return TransformHandler::State::Unlocked;
}

QList<LineResizeTransformHandler::ResizeHandle> LineResizeTransformHandler::getHandles(const std::shared_ptr<LineItem> &item) const
{
    const auto transform{item->transformObj()};
    return {{createHandle(transform.map(item->start()), Common::selectionHandleSize), ResizeHandleType::Start},
            {createHandle(transform.map(item->end()), Common::selectionHandleSize), ResizeHandleType::End}};
}
