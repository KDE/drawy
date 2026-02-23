// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "rotatetransformhandler.hpp"
#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/rotateitemcommand.hpp"
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
#include <algorithm>

using namespace Qt::StringLiterals;

RotateTransformHandler::RotateTransformHandler()
{
    const QPixmap image{u":/cursors/rotate.png"_s};
    m_cursor = QCursor(image.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

bool RotateTransformHandler::shouldActivate(ApplicationContext *context)
{
    const auto [selectionBox, selectionBoxTransform]{context->selectionContext()->selectionBoxWithTransform()};
    const QPointF curPos{context->uiContext()->appEvent()->pos()};

    const QList<QPointF> points{selectionBox.topLeft(), selectionBox.topRight(), selectionBox.bottomRight(), selectionBox.bottomLeft()};
    constexpr qreal rotationHandleSize{50.0};

    auto &transformer{context->spatialContext()->coordinateTransformer()};

    for (const QPointF point : points) {
        if (createHandle(transformer.worldToView(selectionBoxTransform.map(point)), rotationHandleSize).contains(curPos)) {
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

        const auto [selectionBox, selectionBoxTransform]{context->selectionContext()->selectionBoxWithTransform()};
        m_initialAngle = qRadiansToDegrees(Common::Utils::Math::angle(selectionBoxTransform.inverted()));
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

        qreal angle{qRadiansToDegrees(Common::Utils::Math::angle(initialVec, newVec))};
        Q_ASSERT(0 <= angle && angle <= 360);

        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            constexpr std::array<qreal, 17> snapAngles{0, 30, 45, 60, 90, 120, 135, 150, 180, 210, 225, 240, 270, 300, 315, 330, 360};
            const auto lowerBound{std::lower_bound(std::next(snapAngles.begin()), snapAngles.end(), angle)};
            const auto previous{std::prev(lowerBound)};

            const qreal prevDiff{angle - *previous};
            const qreal nextDiff{*lowerBound - angle};

            if (prevDiff < nextDiff) {
                angle = *previous;
            } else {
                angle = *lowerBound;
            }

            const auto prevSnapAngle{std::prev(std::lower_bound(std::next(snapAngles.begin()), snapAngles.end(), m_initialAngle))};

            angle -= (m_initialAngle - *prevSnapAngle);
        }

        if (angle == 0) {
            return TransformHandler::State::Locked;
        }

        QRect dirtyRegion{};

        auto &selectedItems{context->selectionContext()->selectedItems()};
        auto &quadtree{context->spatialContext()->quadtree()};

        for (auto &item : selectedItems) {
            quadtree.deleteItem(item);
            dirtyRegion |= transformer.worldToGrid(item->boundingBox()).toAlignedRect();

            const qreal determinant{item->transformObj().determinant()};
            const qreal itemLastAngle{determinant < 0 ? -m_lastRotationAngle : m_lastRotationAngle};
            const qreal itemAngle{determinant < 0 ? -angle : angle};

            item->rotate(-itemLastAngle, item->transformObj().inverted().map(m_worldCenterPos));
            item->rotate(itemAngle, item->transformObj().inverted().map(m_worldCenterPos));

            if (!item->needsCaching()) {
                item->commitTransformation();
            }

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

        auto &selectedItems{context->selectionContext()->selectedItems()};

        // undo visible rotation
        for (auto &item : selectedItems) {
            item->rotate(-m_lastRotationAngle, item->transformObj().inverted().map(m_worldCenterPos));
        }

        // redo rotation as a command
        auto commandHistory{context->spatialContext()->commandHistory()};

        const QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
        commandHistory->insert(std::make_shared<RotateItemCommand>(items, m_lastRotationAngle, m_worldCenterPos));

        context->renderingContext()->markForRender();
        context->renderingContext()->markForUpdate();
    }

    return TransformHandler::State::Unlocked;
}
