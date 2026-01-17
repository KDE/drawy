// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "movetool.hpp"

#include "canvas/canvas.hpp"
#include "common/renderitems.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "data-structures/quadtree.hpp"
#include "event/event.hpp"
#include "item/item.hpp"

MoveTool::MoveTool()
{
    m_cursor = QCursor(Qt::OpenHandCursor);
}

void MoveTool::mousePressed(ApplicationContext *context)
{
    UIContext *uiContext{context->uiContext()};
    auto button{uiContext->appEvent().button()};

    if (button == Qt::LeftButton || button == Qt::MiddleButton) {
        auto spatialContext{context->spatialContext()};
        auto renderingContext{context->renderingContext()};

        m_isActive = true;

        m_initialOffsetPos = spatialContext->offsetPos();
        m_initialPos = uiContext->appEvent().pos();

        renderingContext->canvas().setCursor(Qt::ClosedHandCursor);
    }
}

void MoveTool::mouseMoved(ApplicationContext *context)
{
    if (m_isActive) {
        auto spatialContext{context->spatialContext()};
        auto renderingContext{context->renderingContext()};
        UIContext *uiContext{context->uiContext()};

        qreal zoom{renderingContext->zoomFactor()};
        QPointF newPoint{m_initialOffsetPos * zoom - uiContext->appEvent().pos() + m_initialPos};

        spatialContext->setOffsetPos(newPoint / zoom);

        renderingContext->markForRender();
        renderingContext->markForUpdate();
    }
}

void MoveTool::mouseReleased(ApplicationContext *context)
{
    UIContext *uiContext{context->uiContext()};
    auto button{uiContext->appEvent().button()};

    if (button == Qt::LeftButton || button == Qt::MiddleButton) {
        auto renderingContext{context->renderingContext()};

        renderingContext->canvas().setCursor(Qt::OpenHandCursor);
        m_isActive = false;
    }
}

Tool::Type MoveTool::type() const
{
    return Tool::Type::Move;
}

QString MoveTool::tooltip() const
{
    return QObject::tr("Move Tool");
}

IconManager::Icon MoveTool::icon() const
{
    return IconManager::Icon::TOOL_MOVE;
}
