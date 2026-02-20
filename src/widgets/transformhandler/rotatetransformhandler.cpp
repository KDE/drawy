#include "rotatetransformhandler.hpp"
#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/moveitemcommand.hpp"
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
#include <QPainter>
#include <QRectF>
#include <qmath.h>
using namespace Qt::StringLiterals;

RotateTransformHandler::RotateTransformHandler()
{
    QPixmap image{u":/cursors/rotate.png"_s};
    m_cursor = QCursor(image.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

bool RotateTransformHandler::shouldActivate(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};

    const auto [selectionBox, selectionBoxTransform]{context->selectionContext()->selectionBoxWithTransform()};
    const QPointF worldPos{transformer.viewToWorld(context->uiContext()->appEvent()->pos())};
    const QPointF relativeCurPos{selectionBoxTransform.inverted().map(worldPos)};

    const QList<QPointF> points{selectionBox.topLeft(), selectionBox.topRight(), selectionBox.bottomRight(), selectionBox.bottomLeft()};
    constexpr qreal rotationHandleSize{50.0};

    for (QPointF point : points) {
        if (createHandle(point, rotationHandleSize).contains(relativeCurPos)) {
            return true;
        }
    }

    return false;
}

TransformHandler::State RotateTransformHandler::mousePressed(ApplicationContext *context)
{
    auto uiContext{context->uiContext()};
    auto event{uiContext->appEvent()};

    if (event->button() == Qt::LeftButton) {
        auto &transformer{context->spatialContext()->coordinateTransformer()};

        m_worldLastPos = transformer.viewToWorld(event->pos());
        m_isActive = true;
        m_worldCenterPos = context->selectionContext()->selectionBox().boundingRect().center();

        m_lastRotationAngle = 0;
    }

    return TransformHandler::State::Locked;
}

TransformHandler::State RotateTransformHandler::mouseMoved(ApplicationContext *context)
{
    auto uiContext{context->uiContext()};
    auto event{uiContext->appEvent()};
    auto transformer{context->spatialContext()->coordinateTransformer()};
    context->renderingContext()->canvas()->setCursor(m_cursor);

    if (m_isActive) {
        const QPointF worldCurPos{transformer.viewToWorld(event->pos())};

        const QPointF initialVec{m_worldLastPos - m_worldCenterPos};
        const QPointF newVec{worldCurPos - m_worldCenterPos};
        const int angle{qRound(qRadiansToDegrees(Common::Utils::Math::angle(initialVec, newVec)))};

        if (angle == 0) {
            return TransformHandler::State::Locked;
        }

        QRect dirtyRegion{};

        auto &selectedItems{context->selectionContext()->selectedItems()};
        auto &quadtree{context->spatialContext()->quadtree()};

        for (auto &item : selectedItems) {
            quadtree.deleteItem(item);
            dirtyRegion |= transformer.worldToGrid(item->boundingBox()).toAlignedRect();

            item->rotate(-m_lastRotationAngle, item->transformObj().inverted().map(m_worldCenterPos));
            item->rotate(angle, item->transformObj().inverted().map(m_worldCenterPos));

            quadtree.insertItem(item);
            dirtyRegion |= transformer.worldToGrid(item->boundingBox()).toAlignedRect();
        }

        m_lastRotationAngle = angle;

        context->renderingContext()->cacheGrid().markDirty(dirtyRegion);
        context->renderingContext()->markForRender();
        context->renderingContext()->markForUpdate();

        return TransformHandler::State::Locked;
    }

    return TransformHandler::State::Unlocked;
}

TransformHandler::State RotateTransformHandler::mouseReleased(ApplicationContext *context)
{
    if (m_isActive) {
        m_isActive = false;
    }

    return TransformHandler::State::Unlocked;
}
