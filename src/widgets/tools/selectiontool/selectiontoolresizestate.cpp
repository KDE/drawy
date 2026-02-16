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

using namespace Qt::StringLiterals;
bool SelectionToolResizeState::mousePressed(ApplicationContext *context)
{
    auto uiContext{context->uiContext()};
    auto event{uiContext->appEvent()};

    if (event->button() == Qt::LeftButton) {
        const auto [rect, transform]{context->selectionContext()->selectionBoxWithTransform()};
        m_initialSelectionBox = rect;
        m_initialSelectionTransform = transform;

        auto &selectedItems{context->selectionContext()->selectedItems()};
        for (auto &item : selectedItems) {
            m_initialTransform[item] = item->transformObj();
        }

        m_isActive = true;
    }

    return true;
}

void SelectionToolResizeState::mouseMoved(ApplicationContext *context)
{
    auto uiContext{context->uiContext()};
    auto event{uiContext->appEvent()};
    auto transformer{context->spatialContext()->coordinateTransformer()};

    const QTransform invTransform{m_initialSelectionTransform.inverted()};

    const int angle{[invTransform] {
        const int curAngle{qRound(Common::Utils::Math::angle(invTransform))};
        return (curAngle >= 0 ? curAngle : 360 + curAngle);
    }()};

    context->renderingContext()->canvas()->setCursor(cursorForHandle(angle));

    if (m_isActive) {
        auto &selectedItems{context->selectionContext()->selectedItems()};
        auto &quadtree{context->spatialContext()->quadtree()};

        const QPointF localCurPos{invTransform.map(transformer.viewToWorld(event->pos()))};
        const auto [newWidth, newHeight, centerOfScale]{[this, prevRect = m_initialSelectionBox, localCurPos] {
            switch (m_handle) {
            case SelectionTool::SelectionHandle::TopRight:
                return topRightHandler(prevRect, localCurPos);
            case SelectionTool::SelectionHandle::Right:
                return rightHandler(prevRect, localCurPos);
            case SelectionTool::SelectionHandle::BottomRight:
                return bottomRightHandler(prevRect, localCurPos);
            case SelectionTool::SelectionHandle::Bottom:
                return bottomHandler(prevRect, localCurPos);
            case SelectionTool::SelectionHandle::BottomLeft:
                return bottomLeftHandler(prevRect, localCurPos);
            case SelectionTool::SelectionHandle::Left:
                return leftHandler(prevRect, localCurPos);
            case SelectionTool::SelectionHandle::TopLeft:
                return topLeftHandler(prevRect, localCurPos);
            case SelectionTool::SelectionHandle::Top:
                return topHandler(prevRect, localCurPos);
            }

            return std::make_tuple(1.0, 1.0, QPointF{0, 0});
        }()};

        const qreal scaleX{newWidth / m_initialSelectionBox.width()};
        const qreal scaleY{newHeight / m_initialSelectionBox.height()};
        const QPointF center{centerOfScale};

        QTransform newTransform{m_initialSelectionTransform};
        newTransform.translate(center.x(), center.y());
        newTransform.scale(scaleX, scaleY);
        newTransform.translate(-center.x(), -center.y());

        QTransform updateTransform{invTransform * newTransform};

        QRectF dirtyRegion{};
        for (auto &item : selectedItems) {
            dirtyRegion |= item->boundingBox();
            quadtree.deleteItem(item);

            item->setTransform(m_initialTransform[item]);
            item->resize(updateTransform);

            dirtyRegion |= item->boundingBox();
            quadtree.insertItem(item);

            item->setDirty(true);
        }

        context->renderingContext()->cacheGrid().markDirty(transformer.worldToGrid(dirtyRegion.toAlignedRect()));
        context->renderingContext()->markForRender();
        context->renderingContext()->markForUpdate();
    }
}

bool SelectionToolResizeState::mouseReleased(ApplicationContext *context)
{
    if (m_isActive) {
        m_isActive = false;

        auto &selectedItems{context->selectionContext()->selectedItems()};
        for (auto &item : selectedItems) {
            item->commitTransformation();
        }

        context->renderingContext()->cacheGrid().markAllDirty();
        context->renderingContext()->markForRender();
        context->renderingContext()->markForUpdate();
    }

    return false;
}

std::tuple<qreal, qreal, QPointF> SelectionToolResizeState::topRightHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectWidth{localCurPos.x() - prevRect.bottomLeft().x()};
    const qreal newRectHeight{prevRect.bottomLeft().y() - localCurPos.y()};

    return std::make_tuple(newRectWidth, newRectHeight, prevRect.bottomLeft());
}

std::tuple<qreal, qreal, QPointF> SelectionToolResizeState::rightHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectWidth{localCurPos.x() - prevRect.bottomLeft().x()};

    return std::make_tuple(newRectWidth, prevRect.height(), prevRect.bottomLeft());
}

std::tuple<qreal, qreal, QPointF> SelectionToolResizeState::bottomRightHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectWidth{localCurPos.x() - prevRect.topLeft().x()};
    const qreal newRectHeight{localCurPos.y() - prevRect.topLeft().y()};

    return std::make_tuple(newRectWidth, newRectHeight, prevRect.topLeft());
}

std::tuple<qreal, qreal, QPointF> SelectionToolResizeState::bottomHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectHeight{localCurPos.y() - prevRect.topLeft().y()};

    return std::make_tuple(prevRect.width(), newRectHeight, prevRect.topLeft());
}

std::tuple<qreal, qreal, QPointF> SelectionToolResizeState::bottomLeftHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectWidth{prevRect.topRight().x() - localCurPos.x()};
    const qreal newRectHeight{localCurPos.y() - prevRect.topRight().y()};

    return std::make_tuple(newRectWidth, newRectHeight, prevRect.topRight());
}

std::tuple<qreal, qreal, QPointF> SelectionToolResizeState::leftHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectWidth{prevRect.topRight().x() - localCurPos.x()};

    return std::make_tuple(newRectWidth, prevRect.height(), prevRect.topRight());
}

std::tuple<qreal, qreal, QPointF> SelectionToolResizeState::topLeftHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectWidth{prevRect.bottomRight().x() - localCurPos.x()};
    const qreal newRectHeight{prevRect.bottomRight().y() - localCurPos.y()};

    return std::make_tuple(newRectWidth, newRectHeight, prevRect.bottomRight());
}

std::tuple<qreal, qreal, QPointF> SelectionToolResizeState::topHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectHeight{prevRect.bottomRight().y() - localCurPos.y()};

    return std::make_tuple(prevRect.width(), newRectHeight, prevRect.bottomRight());
}

void SelectionToolResizeState::setHandle(SelectionTool::SelectionHandle handle)
{
    m_handle = handle;
}

QCursor SelectionToolResizeState::cursorForHandle(const double angle) const
{
    // DO NOT REORDER ANYTHING
    constexpr std::array<int, 8> angles{20, 70, 110, 160, 200, 250, 290, 340};
    constexpr std::array<Qt::CursorShape, 4> cursorShapes{Qt::SizeBDiagCursor, Qt::SizeHorCursor, Qt::SizeFDiagCursor, Qt::SizeVerCursor};

    const int offset{[this] {
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
