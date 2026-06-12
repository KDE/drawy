// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "controller.hpp"

#include <QDateTime>
#include <QPointingDevice>
#include <QWheelEvent>

#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/insertitemcommand.hpp"
#include "common/constants.hpp"
#include "common/renderitems.hpp"
#include "components/toolbar.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "drawy_debug.h"
#include "event/event.hpp"
#include "item/item.hpp"
#include "item/itemcache/itemcache.hpp"
#include "mime/mimemanager.hpp"
#include "tools/texttool.hpp"

Controller::Controller(ApplicationContext *context, QObject *parent)
    : QObject{parent}
    , m_context{context}
    , m_zoomTimer{new QTimer{this}}
{
    connect(m_zoomTimer, &QTimer::timeout, this, &Controller::renderZoom);
}

Controller::~Controller()
{
    qCDebug(DRAWY_LOG) << "Object deleted: Controller";
}

void Controller::mousePressed(QMouseEvent *event)
{
    if (m_zoomInProgress) {
        return;
    }

    // No on really clicks in this corner (0, 0) and this solves a
    // bug on Hyprland where it would register a mouse press in this corner
    if (event->pos() == QPoint{0, 0}) {
        return;
    }

    const qint64 lastTime{m_lastClickTime};
    m_lastClickTime = QDateTime::currentMSecsSinceEpoch();
    if (m_lastClickTime - lastTime <= Common::doubleClickInterval && !m_mouseMoved) {
        m_clickCount++;

        if (m_clickCount == 2) {
            mouseDoubleClick(event);
        } else if (m_clickCount == 3) {
            mouseTripleClick(event);
        }
        return;
    } else {
        m_clickCount = 1;
        m_mouseMoved = false;
    }

    auto contextEvent{m_context->uiContext()->appEvent()};
    auto toolBar{m_context->uiContext()->toolBar()};
    auto canvas{m_context->renderingContext()->canvas()};
    contextEvent->setPos(event->pos(), canvas->scale());
    contextEvent->setButton(event->button());
    contextEvent->setModifiers(event->modifiers());

    if (event->button() == Qt::MiddleButton) {
        m_movingWithMiddleClick = true;
        toolBar->curTool().cleanup();
        toolBar->tool(Tool::Type::Move).mousePressed(m_context);
        return;
    }

    toolBar->curTool().mousePressed(m_context);

    if (event->type() != QEvent::TabletPress) {
        contextEvent->setPressure(1.0);
    }
}

void Controller::mouseDoubleClick(QMouseEvent *event)
{
    if (m_zoomInProgress) {
        return;
    }

    auto contextEvent{m_context->uiContext()->appEvent()};
    auto toolBar{m_context->uiContext()->toolBar()};
    auto canvas{m_context->renderingContext()->canvas()};

    contextEvent->setPos(event->pos(), canvas->scale());
    contextEvent->setButton(event->button());
    contextEvent->setModifiers(event->modifiers());

    toolBar->curTool().mouseDoubleClick(m_context);
}

void Controller::mouseTripleClick(QMouseEvent *event)
{
    if (m_zoomInProgress) {
        return;
    }

    auto contextEvent{m_context->uiContext()->appEvent()};
    auto toolBar{m_context->uiContext()->toolBar()};
    auto canvas{m_context->renderingContext()->canvas()};

    contextEvent->setPos(event->pos(), canvas->scale());
    contextEvent->setButton(event->button());
    contextEvent->setModifiers(event->modifiers());

    toolBar->curTool().mouseTripleClick(m_context);
}

void Controller::mouseMoved(QMouseEvent *event)
{
    if (m_zoomInProgress) {
        return;
    }

    m_mouseMoved = true;

    auto contextEvent{m_context->uiContext()->appEvent()};
    auto toolBar{m_context->uiContext()->toolBar()};
    auto canvas{m_context->renderingContext()->canvas()};

    contextEvent->setPos(event->pos(), canvas->scale());
    contextEvent->setButton(event->button());
    contextEvent->setModifiers(event->modifiers());

    if (m_movingWithMiddleClick) {
        toolBar->tool(Tool::Type::Move).mouseMoved(m_context);
        return;
    }

    toolBar->curTool().mouseMoved(m_context);
}

void Controller::mouseReleased(QMouseEvent *event)
{
    if (m_zoomInProgress) {
        return;
    }

    auto contextEvent{m_context->uiContext()->appEvent()};
    auto toolBar{m_context->uiContext()->toolBar()};
    auto canvas{m_context->renderingContext()->canvas()};

    contextEvent->setPos(event->pos(), canvas->scale());
    contextEvent->setButton(event->button());
    contextEvent->setModifiers(event->modifiers());

    if (event->button() == Qt::MiddleButton) {
        m_movingWithMiddleClick = false;
        toolBar->tool(Tool::Type::Move).mouseReleased(m_context);
        canvas->setCursor(toolBar->curTool().cursor());
        return;
    }

    toolBar->curTool().mouseReleased(m_context);
}

void Controller::tablet(QTabletEvent *event)
{
    if (m_zoomInProgress) {
        return;
    }

    auto ev{m_context->uiContext()->appEvent()};
    auto toolBar{m_context->uiContext()->toolBar()};

    if (!m_usingStylusEraser && event->pointerType() == QPointingDevice::PointerType::Eraser) {
        m_usingStylusEraser = true;
        toolBar->curTool().cleanup();

        const Tool::Type oldTool = toolBar->curTool().type();
        toolBar->changeTool(m_stashedTool);
        m_stashedTool = oldTool;
    }

    if (m_usingStylusEraser && event->pointerType() != QPointingDevice::PointerType::Eraser) {
        m_usingStylusEraser = false;
        toolBar->curTool().cleanup();

        const Tool::Type oldTool = toolBar->curTool().type();
        toolBar->changeTool(m_stashedTool);
        m_stashedTool = oldTool;
    }

    constexpr qreal targetRangeStart{0};
    constexpr qreal targetRangeEnd{1.0};

    // transform the pressure to fit within this range
    ev->setPressure(event->pressure() * (targetRangeEnd - targetRangeStart) + targetRangeStart);

    toolBar->curTool().tablet(m_context);
}

void Controller::keyPressed(QKeyEvent *event)
{
    if (m_zoomInProgress) {
        return;
    }

    auto contextEvent{m_context->uiContext()->appEvent()};
    auto toolBar{m_context->uiContext()->toolBar()};

    contextEvent->setKey(event->key());
    contextEvent->setModifiers(event->modifiers());
    contextEvent->setText(event->text());

    if (event->key() == Qt::Key_Space && !event->isAutoRepeat() && !m_spacePanActive) {
        bool shouldActivateSpacePan{true};
        Tool &curTool{toolBar->curTool()};
        if (curTool.type() == Tool::Type::Text) {
            auto *textTool{dynamic_cast<TextTool *>(&curTool)};
            const std::shared_ptr<TextItem> curItem{textTool == nullptr ? nullptr : textTool->curItem()};

            // we don't want to handle it in case the user is editing text
            if (curItem && curItem->mode() == TextItem::Mode::Edit) {
                shouldActivateSpacePan = false;
            }
        }

        if (shouldActivateSpacePan) {
            m_spacePanPreviousTool = curTool.type();
            m_spacePanActive = true;
            toolBar->changeTool(Tool::Type::Move);
            return;
        }
    }

    toolBar->curTool().keyPressed(m_context);
}

void Controller::keyReleased(QKeyEvent *event)
{
    if (m_zoomInProgress) {
        return;
    }

    auto contextEvent{m_context->uiContext()->appEvent()};
    auto toolBar{m_context->uiContext()->toolBar()};

    contextEvent->setKey(event->key());
    contextEvent->setModifiers(event->modifiers());
    contextEvent->setText(event->text());

    if (event->key() == Qt::Key_Space && !event->isAutoRepeat() && m_spacePanActive) {
        m_spacePanActive = false;
        toolBar->changeTool(m_spacePanPreviousTool);
        return;
    }

    toolBar->curTool().keyReleased(m_context);
}

void Controller::inputMethodInvoked([[maybe_unused]] QInputMethodEvent *event)
{
    if (m_zoomInProgress) {
        return;
    }
}

void Controller::leave([[maybe_unused]] QEvent *event)
{
    if (m_zoomInProgress) {
        return;
    }

    auto toolBar{m_context->uiContext()->toolBar()};

    toolBar->curTool().leave(m_context);
}

void Controller::dragEnter(QDragEnterEvent *event)
{
    if (m_zoomInProgress) {
        return;
    }

    m_droppedItems = m_context->mimeManager()->readData(*event->mimeData());

    if (m_droppedItems.isEmpty()) {
        return;
    }

    event->acceptProposedAction();

    m_context->renderingContext()->canvas()->setOverlayBg(Qt::transparent);
    dragMove(event);
}

void Controller::dragMove(QDragMoveEvent *event)
{
    if (m_zoomInProgress) {
        return;
    }

    auto *canvas{m_context->renderingContext()->canvas()};

    auto contextEvent{m_context->uiContext()->appEvent()};
    contextEvent->setPos(event->position().toPoint(), canvas->scale());
    contextEvent->setModifiers(event->modifiers());

    const QPointF offsetPos{m_context->spatialContext()->offsetPos()};
    const qreal zoom{m_context->renderingContext()->zoomFactor()};

    canvas->paintOverlay([&](QPainter &painter) -> void {
        painter.scale(zoom, zoom);

        for (const auto &item : std::as_const(m_droppedItems)) {
            item->erase(painter, offsetPos);
        }
    });

    QRectF boundingBox;

    for (const auto &item : std::as_const(m_droppedItems)) {
        boundingBox |= item->boundingBox();
    }

    auto transformer{m_context->spatialContext()->coordinateTransformer()};

    for (const auto &item : std::as_const(m_droppedItems)) {
        const auto transform{item->transformObj().inverted()};

        const auto localInitialPos{transform.map(item->boundingBox().center())};
        const auto localFinalPos{transform.map(transformer.viewToWorld(contextEvent->pos()))};

        const auto localDelta{localFinalPos - localInitialPos};

        item->translate(localDelta);
    }

    canvas->paintOverlay([&](QPainter &painter) -> void {
        painter.scale(zoom, zoom);

        painter.translate(-offsetPos);

        for (const auto &item : std::as_const(m_droppedItems)) {
            painter.save();
            painter.setTransform(item->transformObj(), true);
            item->draw(painter, QPointF{0, 0});
            painter.restore();
        }
    });

    m_context->renderingContext()->markForUpdate();
}

void Controller::dragLeave()
{
    if (m_zoomInProgress) {
        return;
    }

    auto *canvas{m_context->renderingContext()->canvas()};

    const QPointF offsetPos{m_context->spatialContext()->offsetPos()};
    const qreal zoom{m_context->renderingContext()->zoomFactor()};

    canvas->paintOverlay([&](QPainter &painter) -> void {
        painter.scale(zoom, zoom);

        for (const auto &item : std::as_const(m_droppedItems)) {
            item->erase(painter, offsetPos);
        }
    });

    m_droppedItems = {};

    m_context->renderingContext()->markForUpdate();
}

void Controller::drop()
{
    if (m_zoomInProgress) {
        return;
    }

    auto commandHistory{m_context->spatialContext()->commandHistory()};
    commandHistory->push(std::make_shared<InsertItemCommand>(m_droppedItems));

    m_context->selectionContext()->setSelectedItems(m_droppedItems.begin(), m_droppedItems.end());

    dragLeave();

    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void Controller::renderZoom()
{
    if (m_zoomDelta == 0) {
        return;
    }

    auto renderingContext{m_context->renderingContext()};
    auto spatialContext{m_context->spatialContext()};

    const qreal oldZoomFactor{renderingContext->zoomFactor()};
    const qreal relFactor{std::pow(Common::zoomMultiplier, m_zoomDelta)};
    const qreal newZoomFactor{oldZoomFactor * relFactor};

    renderingContext->setZoomFactor(newZoomFactor);
    spatialContext->setOffsetPos(m_zoomPixmapOffsetPos / oldZoomFactor + spatialContext->offsetPos());

    renderingContext->cacheGrid().markAllDirty();
    renderingContext->itemCache().clear();
    renderingContext->markForRender();
    renderingContext->markForUpdate();

    connect(
        renderingContext,
        &RenderingContext::canvasRendered,
        this,
        [this]() -> void {
            m_zoomInProgress = false;
        },
        Qt::SingleShotConnection);

    m_zoomDelta = 0;
    m_zoomPixmap = QPixmap{};
    m_zoomPixmapOffsetPos = QPointF{0, 0};
}

void Controller::wheel(QWheelEvent *event)
{
    const QPointF &offsetPos{m_context->spatialContext()->offsetPos()};
    auto canvas{m_context->renderingContext()->canvas()};
    auto contextEvent{m_context->uiContext()->appEvent()};
    const qreal zoomFactor{m_context->renderingContext()->zoomFactor()};

    contextEvent->setPos(event->position().toPoint(), canvas->scale());
    contextEvent->setModifiers(event->modifiers());

    if (canvas->overlayDirty()) {
        canvas->setOverlayBg(canvas->overlayBg());
    }

    if (event->modifiers() & Qt::ControlModifier) {
        if (m_zoomPixmap.isNull()) {
            m_zoomPixmap = canvas->canvasPixmap();
        }

        const int curDelta{event->angleDelta().y() > 0 ? 1 : -1};
        const qreal oldZoomFactor{std::pow(Common::zoomMultiplier, m_zoomDelta)};
        const qreal newZoomFactor{std::pow(Common::zoomMultiplier, m_zoomDelta + curDelta)};
        const qreal prevZoomFactor{m_context->renderingContext()->zoomFactor() * oldZoomFactor};

        if (curDelta < 0 && prevZoomFactor - Common::zoomOutLimit <= 1e-9) {
            return;
        }

        if (curDelta > 0 && Common::zoomInLimit - prevZoomFactor <= 1e-9) {
            return;
        }

        m_zoomDelta += curDelta;
        const QPointF &cursor{contextEvent->pos() / oldZoomFactor + m_zoomPixmapOffsetPos}; // converting to pixmap coordinates

        // calculate new offset position based on cursor position
        m_zoomPixmapOffsetPos.setX(cursor.x() - (cursor.x() - m_zoomPixmapOffsetPos.x()) * oldZoomFactor / newZoomFactor);
        m_zoomPixmapOffsetPos.setY(cursor.y() - (cursor.y() - m_zoomPixmapOffsetPos.y()) * oldZoomFactor / newZoomFactor);

        canvas->setCanvasBg(canvas->canvasBg());
        canvas->paintCanvas([&](QPainter &painter) -> void {
            painter.scale(newZoomFactor, newZoomFactor);
            painter.drawPixmap(-m_zoomPixmapOffsetPos, m_zoomPixmap);
        });

        m_context->renderingContext()->markForUpdate();

        m_zoomInProgress = true;
        m_zoomTimer->start(Common::zoomRenderWaitTime);
        return;
    }

    if (m_zoomInProgress) {
        return;
    }

    m_context->spatialContext()->setOffsetPos(offsetPos - event->angleDelta() / zoomFactor);

    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

#include "moc_controller.cpp"
