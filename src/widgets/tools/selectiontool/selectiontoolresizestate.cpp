// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "selectiontoolresizestate.hpp"

#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/deselectcommand.hpp"
#include "command/selectcommand.hpp"
#include "common/utils/math.hpp"
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
#include <algorithm>
#include <qnamespace.h>

using namespace Qt::StringLiterals;
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

    const QTransform selectionTransform{context->selectionContext()->selectionBoxWithTransform().second.inverted()};
    const int angle{[selectionTransform] {
        const int curAngle{qRound(Common::Utils::Math::angle(selectionTransform))};
        return (curAngle >= 0 ? curAngle : 360 + curAngle);
    }()};

    context->renderingContext()->canvas()->setCursor(cursorForHandle(angle));

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

void SelectionToolResizeState::setHandle(SelectionTool::SelectionHandle handle)
{
    m_handle = handle;
}

QCursor SelectionToolResizeState::cursorForHandle(double angle) const
{
    // DO NOT REORDER ANYTHING
    constexpr std::array<int, 8> angles{20, 70, 110, 160, 200, 250, 290, 340};
    constexpr std::array<Qt::CursorShape, 4> cursorShapes{Qt::SizeBDiagCursor, Qt::SizeHorCursor, Qt::SizeFDiagCursor, Qt::SizeVerCursor};

    int offset{[this] {
        switch (m_handle) {
        case SelectionTool::SelectionHandle::TopRight:
            return 0;
        case SelectionTool::SelectionHandle::Right:
            return 1;
        case SelectionTool::SelectionHandle::BottomRight:
            return 2;
        case SelectionTool::SelectionHandle::Bottom:
            return 3;
        case SelectionTool::SelectionHandle::BottomLeft:
            return 4;
        case SelectionTool::SelectionHandle::Left:
            return 5;
        case SelectionTool::SelectionHandle::TopLeft:
            return 6;
        case SelectionTool::SelectionHandle::Top:
            return 7;
        }

        return 0;
    }()};

    const std::size_t cursorIndex{static_cast<std::size_t>(std::lower_bound(angles.begin(), angles.end(), angle) - angles.begin()) + offset};
    return cursorShapes.at(cursorIndex % 4);
}
