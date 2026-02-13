// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "selectiontoolrotatestate.hpp"

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

using namespace Qt::StringLiterals;
SelectionToolRotateState::SelectionToolRotateState()
{
    QPixmap image{u":/cursors/rotate.png"_s};
    m_cursor = QCursor(image.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

bool SelectionToolRotateState::mousePressed(ApplicationContext *context)
{
    auto uiContext{context->uiContext()};
    auto event{uiContext->appEvent()};

    if (event->button() == Qt::LeftButton) {
        m_lastPoint = event->pos();
        m_isActive = true;
        m_centerPoint = context->selectionContext()->selectionBox().boundingRect().center();
    }

    return true;
}

void SelectionToolRotateState::mouseMoved(ApplicationContext *context)
{
    auto uiContext{context->uiContext()};
    auto event{uiContext->appEvent()};
    auto transformer{context->spatialContext()->coordinateTransformer()};
    context->renderingContext()->canvas()->setCursor(m_cursor);

    // distance between two points
    constexpr auto dist = [](const QPointF first, const QPointF second) -> qreal {
        return qSqrt(qPow(first.x() - second.x(), 2) + qPow(first.y() - second.y(), 2));
    };

    if (m_isActive) {
        const QPointF curPoint{event->pos()};

        const qreal perp{dist(m_lastPoint, curPoint)};
        const qreal radius{dist(m_lastPoint, transformer.worldToView(m_centerPoint))};
        const int angle{qRound(qAtan(perp / radius) * 180 / M_PI)};

        if (angle == 0) {
            return;
        }

        QRect dirtyRegion{};

        auto &selectedItems{context->selectionContext()->selectedItems()};
        for (auto &item : selectedItems) {
            item->rotate(angle, item->transformObj().inverted().map(m_centerPoint));
            dirtyRegion |= transformer.worldToGrid(item->boundingBox()).toAlignedRect();
        }

        /*
        context->renderingContext()->canvas()->paintOverlay([&](QPainter &painter) -> void {
            painter.setPen(Qt::red);
            painter.setBrush(Qt::red);
            painter.drawEllipse(m_centerPoint, 10, 10);
            painter.drawEllipse(curPoint, 10, 10);
            painter.drawLine(m_lastPoint, m_centerPoint);
        });
        */

        context->renderingContext()->cacheGrid().markDirty(dirtyRegion);
        context->renderingContext()->markForRender();
        context->renderingContext()->markForUpdate();

        m_lastPoint = curPoint;
    }
}

bool SelectionToolRotateState::mouseReleased(ApplicationContext *context)
{
    if (m_isActive) {
        m_isActive = false;
    }

    return false;
}
