// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "lineresizetransformhandler.hpp"
#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/lineresizeitemcommand.hpp"
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
#include <stdexcept>

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
        painter.setPen(Common::selectionBorderPen());
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

        auto item{getSelectedLineItem(context)};

        m_initialPos = [&item, this] {
            switch (m_curHandleType) {
            case ResizeHandleType::Start:
                return item->start();
            case ResizeHandleType::End:
                return item->end();
            default:
                return QPointF{};
            }
        }();

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

        m_lastPos = relativeCurPos;

        renderingContext->cacheGrid().markDirty(transformer.worldToGrid(dirtyRegion.toAlignedRect()));
        renderingContext->markForRender();
        renderingContext->markForUpdate();

        return TransformHandler::State::Locked;
    }

    return TransformHandler::State::Unlocked;
}

TransformHandler::State LineResizeTransformHandler::mouseReleased(ApplicationContext *context)
{
    m_isActive = false;

    const LineResizeItemCommand::HandleType handleType{[this] {
        switch (m_curHandleType) {
        case ResizeHandleType::Start:
            return LineResizeItemCommand::HandleType::Start;
        case ResizeHandleType::End:
            return LineResizeItemCommand::HandleType::End;
        default: {
            throw std::logic_error("Incorrect line resize handle selected! Please report this as a bug.");
        };
        }
    }()};

    auto item{getSelectedLineItem(context)};

    auto commandHistory{context->spatialContext()->commandHistory()};
    commandHistory->insert(std::make_shared<LineResizeItemCommand>(QList<std::shared_ptr<Item>>{item}, m_initialPos, m_lastPos, handleType));

    return TransformHandler::State::Unlocked;
}

QList<LineResizeTransformHandler::ResizeHandle> LineResizeTransformHandler::getHandles(const std::shared_ptr<LineItem> &item) const
{
    const auto transform{item->transformObj()};
    return {{createHandle(transform.map(item->start()), Common::selectionHandleSize), ResizeHandleType::Start},
            {createHandle(transform.map(item->end()), Common::selectionHandleSize), ResizeHandleType::End}};
}

#include "moc_lineresizetransformhandler.cpp"
