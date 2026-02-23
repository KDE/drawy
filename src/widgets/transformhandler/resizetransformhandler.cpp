// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "resizetransformhandler.hpp"
#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/resizeitemcommand.hpp"
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
#include <QPainter>
#include <QRectF>

using namespace Qt::StringLiterals;

void ResizeTransformHandler::renderHandles(ApplicationContext *context)
{
    const auto &selectedItems{context->selectionContext()->selectedItems()};
    if (selectedItems.empty()) {
        return;
    }

    const auto viewSelectionBox{context->spatialContext()->coordinateTransformer().worldToView(context->selectionContext()->selectionBox())};

    context->renderingContext()->canvas()->paintCanvas([viewSelectionBox, context](QPainter &painter) {
        painter.setPen(Common::selectionBorderPen());
        painter.setBrush(context->renderingContext()->canvas()->canvasBg());

        for (const QPointF &point : viewSelectionBox) {
            painter.drawRect(createHandle(point, Common::selectionHandleSize));
        }
    });
}

bool ResizeTransformHandler::shouldActivate(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    const auto [selectionBox, selectionBoxTransform]{context->selectionContext()->selectionBoxWithTransform()};
    const auto relativeCurPos{selectionBoxTransform.inverted().map(transformer.viewToWorld(context->uiContext()->appEvent()->pos()))};

    const auto handles{getHandles(selectionBox, context->renderingContext()->zoomFactor())};
    return std::ranges::any_of(handles, [&relativeCurPos](const auto &handle) {
        return handle.rect.contains(relativeCurPos);
    });
}

TransformHandler::State ResizeTransformHandler::mousePressed(ApplicationContext *context)
{
    if (context->uiContext()->appEvent()->button() == Qt::LeftButton) {
        const auto [selectionBox, selectionBoxTransform]{context->selectionContext()->selectionBoxWithTransform()};

        m_initialSelectionBox = selectionBox;
        m_initialSelectionTransform = selectionBoxTransform;
        m_lastTransformUpdate = {};
        m_lastLockedTransformUpdate = {};
        m_aspectRatioLocked.clear();

        m_isActive = true;
    }

    return TransformHandler::State::Locked;
}

TransformHandler::State ResizeTransformHandler::mouseMoved(ApplicationContext *context)
{
    auto uiContext{context->uiContext()};
    auto transformer{context->spatialContext()->coordinateTransformer()};
    const auto [selectionBox, selectionBoxTransform]{context->selectionContext()->selectionBoxWithTransform()};

    if (!m_isActive) {
        const auto worldPos{transformer.viewToWorld(uiContext->appEvent()->pos())};
        const auto relativeCurPos{selectionBoxTransform.inverted().map(worldPos)};

        const auto handles{getHandles(selectionBox, context->renderingContext()->zoomFactor())};
        auto it{std::ranges::find_if(handles, [&relativeCurPos](const auto &handle) {
            return handle.rect.contains(relativeCurPos);
        })};

        if (it != handles.end()) {
            m_activeHandleType = it->type;
        }
    }

    const QTransform invTransform{m_initialSelectionTransform.inverted()};
    const int angle{qRound(qRadiansToDegrees((Common::Utils::Math::angle(invTransform))))};

    context->renderingContext()->canvas()->setCursor(cursorForHandle(angle));

    if (m_isActive) {
        auto &selectedItems{context->selectionContext()->selectedItems()};
        auto &quadtree{context->spatialContext()->quadtree()};

        const QPointF localCurPos{invTransform.map(transformer.viewToWorld(uiContext->appEvent()->pos()))};
        const auto [newWidth, newHeight, centerOfScale]{[this, prevRect = m_initialSelectionBox, localCurPos] {
            switch (m_activeHandleType) {
            case ResizeHandleType::TopRight:
                return topRightHandler(prevRect, localCurPos);
            case ResizeHandleType::Right:
                return rightHandler(prevRect, localCurPos);
            case ResizeHandleType::BottomRight:
                return bottomRightHandler(prevRect, localCurPos);
            case ResizeHandleType::Bottom:
                return bottomHandler(prevRect, localCurPos);
            case ResizeHandleType::BottomLeft:
                return bottomLeftHandler(prevRect, localCurPos);
            case ResizeHandleType::Left:
                return leftHandler(prevRect, localCurPos);
            case ResizeHandleType::TopLeft:
                return topLeftHandler(prevRect, localCurPos);
            case ResizeHandleType::Top:
                return topHandler(prevRect, localCurPos);
            }
            return std::make_tuple(1.0, 1.0, QPointF{0, 0});
        }()};

        const qreal scaleX{newWidth / m_initialSelectionBox.width()};
        const qreal scaleY{newHeight / m_initialSelectionBox.height()};

        QTransform newTransform{m_initialSelectionTransform};
        newTransform.translate(centerOfScale.x(), centerOfScale.y());
        newTransform.scale(scaleX, scaleY);
        newTransform.translate(-centerOfScale.x(), -centerOfScale.y());

        qreal scaleLocked;
        if (std::abs(scaleX - 1.0) > std::abs(scaleY - 1.0)) {
            scaleLocked = scaleX;
        } else {
            scaleLocked = scaleY;
        }

        QTransform newTransformLocked{m_initialSelectionTransform};
        newTransformLocked.translate(centerOfScale.x(), centerOfScale.y());
        newTransformLocked.scale(scaleLocked, scaleLocked);
        newTransformLocked.translate(-centerOfScale.x(), -centerOfScale.y());

        // we don't want the scale to ever be 0
        if (newTransform.m11() == 0 || newTransform.m22() == 0) {
            return TransformHandler::State::Locked;
        }

        const QTransform transformUpdate{invTransform * newTransform};
        const QTransform lockedTransformUpdate{invTransform * newTransformLocked};

        const bool useLocked{uiContext->appEvent()->modifiers().testFlag(Qt::ShiftModifier) || selectedItems.size() > 1};

        QRectF dirtyRegion{};
        for (const auto &item : selectedItems) {
            dirtyRegion |= item->boundingBox();
            quadtree.deleteItem(item);

            if (item->lockAspectRatioWhenResizing() || useLocked) {
                item->resize(m_lastLockedTransformUpdate.inverted());
                item->resize(lockedTransformUpdate);
                m_aspectRatioLocked[item] = true;
            } else {
                item->resize(m_lastTransformUpdate.inverted());
                item->resize(transformUpdate);
            }

            if (!item->needsCaching()) {
                item->commitTransformation();
            }

            dirtyRegion |= item->boundingBox();
            quadtree.insertItem(item);

            item->setDirty(true);
        }

        context->renderingContext()->cacheGrid().markDirty(transformer.worldToGrid(dirtyRegion.toAlignedRect()));
        context->renderingContext()->markForRender();
        context->renderingContext()->markForUpdate();

        m_lastTransformUpdate = transformUpdate;
        m_lastLockedTransformUpdate = lockedTransformUpdate;
        return TransformHandler::State::Locked;
    }

    return TransformHandler::State::Unlocked;
}

TransformHandler::State ResizeTransformHandler::mouseReleased(ApplicationContext *context)
{
    if (m_isActive) {
        m_isActive = false;

        const auto &selectedItems{context->selectionContext()->selectedItems()};

        // undo resize
        for (const auto &item : selectedItems) {
            if (m_aspectRatioLocked[item]) {
                item->resize(m_lastLockedTransformUpdate.inverted());
            } else {
                item->resize(m_lastTransformUpdate.inverted());
            }
        }

        auto commandHistory{context->spatialContext()->commandHistory()};

        const QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
        commandHistory->insert(std::make_shared<ResizeItemCommand>(items, m_lastTransformUpdate, m_lastLockedTransformUpdate, m_aspectRatioLocked));

        context->renderingContext()->markForRender();
        context->renderingContext()->markForUpdate();
    }

    return TransformHandler::State::Unlocked;
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::topRightHandler(const QRectF prevRect, const QPointF localCurPos)
{
    return {localCurPos.x() - prevRect.bottomLeft().x(), prevRect.bottomLeft().y() - localCurPos.y(), prevRect.bottomLeft()};
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::rightHandler(const QRectF prevRect, const QPointF localCurPos)
{
    return {localCurPos.x() - prevRect.bottomLeft().x(), prevRect.height(), QPointF{prevRect.left(), prevRect.center().y()}};
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::bottomRightHandler(const QRectF prevRect, const QPointF localCurPos)
{
    return {localCurPos.x() - prevRect.topLeft().x(), localCurPos.y() - prevRect.topLeft().y(), prevRect.topLeft()};
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::bottomHandler(const QRectF prevRect, const QPointF localCurPos)
{
    return {prevRect.width(), localCurPos.y() - prevRect.topLeft().y(), QPointF{prevRect.center().x(), prevRect.top()}};
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::bottomLeftHandler(const QRectF prevRect, const QPointF localCurPos)
{
    return {prevRect.topRight().x() - localCurPos.x(), localCurPos.y() - prevRect.topRight().y(), prevRect.topRight()};
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::leftHandler(const QRectF prevRect, const QPointF localCurPos)
{
    return {prevRect.topRight().x() - localCurPos.x(), prevRect.height(), QPointF{prevRect.right(), prevRect.center().y()}};
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::topLeftHandler(const QRectF prevRect, const QPointF localCurPos)
{
    return {prevRect.bottomRight().x() - localCurPos.x(), prevRect.bottomRight().y() - localCurPos.y(), prevRect.bottomRight()};
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::topHandler(const QRectF prevRect, const QPointF localCurPos)
{
    return {prevRect.width(), prevRect.bottomRight().y() - localCurPos.y(), QPointF{prevRect.center().x(), prevRect.bottom()}};
}

QList<ResizeTransformHandler::ResizeHandle> ResizeTransformHandler::getHandles(const QRectF selectionBox, const qreal zoom)
{
    const qreal handleSize{20.0 / zoom};
    const qreal handleSizeHalf{handleSize / 2.0};

    return {
        {createHandle(selectionBox.topLeft(), handleSize), ResizeHandleType::TopLeft},
        {createHandle(selectionBox.topRight(), handleSize), ResizeHandleType::TopRight},
        {createHandle(selectionBox.bottomRight(), handleSize), ResizeHandleType::BottomRight},
        {createHandle(selectionBox.bottomLeft(), handleSize), ResizeHandleType::BottomLeft},
        {QRectF{selectionBox.left(), selectionBox.top() - handleSizeHalf, selectionBox.width(), handleSize}, ResizeHandleType::Top},
        {QRectF{selectionBox.right() - handleSizeHalf, selectionBox.top(), handleSize, selectionBox.height()}, ResizeHandleType::Right},
        {QRectF{selectionBox.left(), selectionBox.bottom() - handleSizeHalf, selectionBox.width(), handleSize}, ResizeHandleType::Bottom},
        {QRectF{selectionBox.left() - handleSizeHalf, selectionBox.top(), handleSize, selectionBox.height()}, ResizeHandleType::Left},
    };
}

QCursor ResizeTransformHandler::cursorForHandle(const double angle) const
{
    constexpr std::array<int, 8> angles{20, 70, 110, 160, 200, 250, 290, 340};
    std::array<Qt::CursorShape, 4> cursorShapes{Qt::SizeBDiagCursor, Qt::SizeHorCursor, Qt::SizeFDiagCursor, Qt::SizeVerCursor};

    const int offset{[this] {
        switch (m_activeHandleType) {
        case ResizeHandleType::TopRight:
            return 0;
        case ResizeHandleType::Right:
            return 1;
        case ResizeHandleType::BottomRight:
            return 2;
        case ResizeHandleType::Bottom:
            return 3;
        case ResizeHandleType::BottomLeft:
            return 4;
        case ResizeHandleType::Left:
            return 5;
        case ResizeHandleType::TopLeft:
            return 6;
        case ResizeHandleType::Top:
            return 7;
        }
        return 0;
    }()};

    const std::size_t cursorIndex{static_cast<std::size_t>(std::lower_bound(angles.begin(), angles.end(), angle) - angles.begin()) + offset};
    return cursorShapes.at(cursorIndex % 4);
}

#include "moc_resizetransformhandler.cpp"
