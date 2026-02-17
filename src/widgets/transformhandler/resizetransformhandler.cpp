#include "resizetransformhandler.hpp"
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
#include <QPainter>
#include <QRectF>

using namespace Qt::StringLiterals;

void ResizeTransformHandler::renderHandles(ApplicationContext *context)
{
    const auto &selectedItems{context->selectionContext()->selectedItems()};

    if (selectedItems.empty()) {
        return;
    }

    auto &transformer{context->spatialContext()->coordinateTransformer()};
    const QPolygonF viewSelectionBox{transformer.worldToView(context->selectionContext()->selectionBox())};

    context->renderingContext()->canvas()->paintCanvas([viewSelectionBox, context](QPainter &painter) -> void {
        painter.setPen(Common::selectionBorderColor);
        painter.setBrush(context->renderingContext()->canvas()->canvasBg());

        constexpr qreal handleWidth{10.0}, handleWidthHalf{handleWidth / 2.0};
        for (QPointF point : viewSelectionBox) {
            painter.drawRect(QRectF{point.x() - handleWidthHalf, point.y() - handleWidthHalf, handleWidth, handleWidth});
        }
    });
}

bool ResizeTransformHandler::shouldActivate(const QRectF selectionBox, const QPointF relativeCurPos)
{
    const auto &resizeHandles{getHandles(selectionBox)};

    for (const auto &[handle, handleType] : resizeHandles) {
        if (handle.contains(relativeCurPos)) {
            return true;
        }
    }

    return false;
}

TransformHandler::State ResizeTransformHandler::mousePressed(ApplicationContext *context)
{
    auto uiContext{context->uiContext()};
    auto event{uiContext->appEvent()};

    if (event->button() == Qt::LeftButton) {
        const auto [selectionBox, selectionBoxTransform]{context->selectionContext()->selectionBoxWithTransform()};

        m_initialSelectionBox = selectionBox;
        m_initialSelectionTransform = selectionBoxTransform;

        auto &selectedItems{context->selectionContext()->selectedItems()};
        for (auto &item : selectedItems) {
            m_initialTransform[item] = item->transformObj();
        }

        m_isActive = true;
    }

    return TransformHandler::State::Locked;
}

TransformHandler::State ResizeTransformHandler::mouseMoved(ApplicationContext *context)
{
    auto uiContext{context->uiContext()};
    auto event{uiContext->appEvent()};
    auto transformer{context->spatialContext()->coordinateTransformer()};

    const auto [selectionBox, selectionBoxTransform]{context->selectionContext()->selectionBoxWithTransform()};

    if (!m_isActive) {
        const auto worldPos{transformer.viewToWorld(uiContext->appEvent()->pos())};
        const auto relativeCurPos{selectionBoxTransform.inverted().map(worldPos)};

        const auto &resizeHandles{getHandles(selectionBox)};
        for (const auto &[handle, handleType] : resizeHandles) {
            if (handle.contains(relativeCurPos)) {
                m_activeHandleType = handleType;
                break;
            }
        }
    }

    const int angle{[invTransform = selectionBoxTransform.inverted()] {
        const int curAngle{qRound(Common::Utils::Math::angle(invTransform))};
        return (curAngle >= 0 ? curAngle : 360 + curAngle);
    }()};

    const QTransform invTransform{m_initialSelectionTransform.inverted()};

    context->renderingContext()->canvas()->setCursor(cursorForHandle(angle));

    if (m_isActive) {
        auto &selectedItems{context->selectionContext()->selectedItems()};
        auto &quadtree{context->spatialContext()->quadtree()};

        const QPointF localCurPos{invTransform.map(transformer.viewToWorld(event->pos()))};
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

        return TransformHandler::State::Locked;
    }

    return TransformHandler::State::Unlocked;
}

TransformHandler::State ResizeTransformHandler::mouseReleased(ApplicationContext *context)
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

    return TransformHandler::State::Unlocked;
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::topRightHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectWidth{localCurPos.x() - prevRect.bottomLeft().x()};
    const qreal newRectHeight{prevRect.bottomLeft().y() - localCurPos.y()};

    return std::make_tuple(newRectWidth, newRectHeight, prevRect.bottomLeft());
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::rightHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectWidth{localCurPos.x() - prevRect.bottomLeft().x()};

    return std::make_tuple(newRectWidth, prevRect.height(), prevRect.bottomLeft());
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::bottomRightHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectWidth{localCurPos.x() - prevRect.topLeft().x()};
    const qreal newRectHeight{localCurPos.y() - prevRect.topLeft().y()};

    return std::make_tuple(newRectWidth, newRectHeight, prevRect.topLeft());
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::bottomHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectHeight{localCurPos.y() - prevRect.topLeft().y()};

    return std::make_tuple(prevRect.width(), newRectHeight, prevRect.topLeft());
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::bottomLeftHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectWidth{prevRect.topRight().x() - localCurPos.x()};
    const qreal newRectHeight{localCurPos.y() - prevRect.topRight().y()};

    return std::make_tuple(newRectWidth, newRectHeight, prevRect.topRight());
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::leftHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectWidth{prevRect.topRight().x() - localCurPos.x()};

    return std::make_tuple(newRectWidth, prevRect.height(), prevRect.topRight());
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::topLeftHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectWidth{prevRect.bottomRight().x() - localCurPos.x()};
    const qreal newRectHeight{prevRect.bottomRight().y() - localCurPos.y()};

    return std::make_tuple(newRectWidth, newRectHeight, prevRect.bottomRight());
}

std::tuple<qreal, qreal, QPointF> ResizeTransformHandler::topHandler(const QRectF prevRect, const QPointF localCurPos)
{
    const qreal newRectHeight{prevRect.bottomRight().y() - localCurPos.y()};

    return std::make_tuple(prevRect.width(), newRectHeight, prevRect.bottomRight());
}

constexpr QList<ResizeTransformHandler::ResizeHandle> ResizeTransformHandler::getHandles(const QRectF selectionBox)
{
    constexpr auto createHandle = [](const QPointF point, const qreal size) -> QRectF {
        return QRectF{point.x() - size / 2.0, point.y() - size / 2.0, size, size};
    };

    constexpr qreal handleSize{20.0};
    constexpr qreal handleSizeHalf{handleSize / 2.0};

    const QList<ResizeHandle> resizeHandles{{
        {createHandle(selectionBox.topLeft(), handleSize), ResizeHandleType::TopLeft},
        {createHandle(selectionBox.topRight(), handleSize), ResizeHandleType::TopRight},
        {createHandle(selectionBox.bottomRight(), handleSize), ResizeHandleType::BottomRight},
        {createHandle(selectionBox.bottomLeft(), handleSize), ResizeHandleType::BottomLeft},
        {QRectF{selectionBox.left(), selectionBox.top() - handleSizeHalf, selectionBox.width(), handleSize}, ResizeHandleType::Top},
        {QRectF{selectionBox.right() - handleSizeHalf, selectionBox.top(), handleSize, selectionBox.height()}, ResizeHandleType::Right},
        {QRectF{selectionBox.left(), selectionBox.bottom() - handleSizeHalf, selectionBox.width(), handleSize}, ResizeHandleType::Bottom},
        {QRectF{selectionBox.left() - handleSizeHalf, selectionBox.top(), handleSize, selectionBox.height()}, ResizeHandleType::Left},
    }};

    return resizeHandles;
}

QCursor ResizeTransformHandler::cursorForHandle(const double angle) const
{
    // DO NOT REORDER ANYTHING
    constexpr std::array<int, 8> angles{20, 70, 110, 160, 200, 250, 290, 340};
    constexpr std::array<Qt::CursorShape, 4> cursorShapes{Qt::SizeBDiagCursor, Qt::SizeHorCursor, Qt::SizeFDiagCursor, Qt::SizeVerCursor};

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
