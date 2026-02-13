// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "selectiontoolresizestate.hpp"

#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/deselectcommand.hpp"
#include "command/selectcommand.hpp"
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
#include <QtMath>

bool SelectionToolResizeState::mousePressed(ApplicationContext *context)
{
    auto uiContext{context->uiContext()};
    auto event{uiContext->appEvent()};

    if (event->button() == Qt::LeftButton) { }

    return true;
}

void SelectionToolResizeState::mouseMoved(ApplicationContext *context)
{
    auto uiContext{context->uiContext()};
    auto event{uiContext->appEvent()};
    auto transformer{context->spatialContext()->coordinateTransformer()};
    context->renderingContext()->canvas()->setCursor(Qt::SizeHorCursor);

    if (m_isActive) {
        const QPointF viewCurPoint{event->pos()};
        const QPointF viewDiff{viewCurPoint - m_viewLastPoint};
    }
}

bool SelectionToolResizeState::mouseReleased(ApplicationContext *context)
{
    if (m_isActive) {
        m_isActive = false;
    }

    return false;
}
