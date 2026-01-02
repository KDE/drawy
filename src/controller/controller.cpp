// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "controller.hpp"

#include <QWheelEvent>

#include "../canvas/canvas.hpp"
#include "../common/constants.hpp"
#include "../common/renderitems.hpp"
#include "../components/toolbar.hpp"
#include "../context/applicationcontext.hpp"
#include "../context/coordinatetransformer.hpp"
#include "../context/renderingcontext.hpp"
#include "../context/spatialcontext.hpp"
#include "../context/uicontext.hpp"
#include "../event/event.hpp"
#include "drawy_debug.h"

Controller::Controller(QObject *parent)
    : QObject{parent}
    , m_context(ApplicationContext::instance(dynamic_cast<QWidget *>(parent)))
{
    m_context->setContexts();
}

Controller::~Controller()
{
    qCDebug(DRAWY_LOG) << "Object deleted: Controller";
}

void Controller::mousePressed(QMouseEvent *event)
{
    // No on really clicks in this corner (0, 0) and this solves a
    // bug on Hyprland where it would register a mouse press in this corner
    if (event->pos() == QPoint{0, 0})
        return;

    qint64 lastTime{m_lastClickTime};
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

    Event *contextEvent{m_context->uiContext()->event()};
    ToolBar *toolBar{m_context->uiContext()->toolBar()};
    Canvas *canvas{m_context->renderingContext()->canvas()};

    contextEvent->setPos(event->pos(), canvas->scale());
    contextEvent->setButton(event->button());
    contextEvent->setModifiers(event->modifiers());

    if (event->button() == Qt::MiddleButton) {
        m_movingWithMiddleClick = true;
        toolBar->curTool().cleanup();
        toolBar->tool(Tool::Move).mousePressed(m_context);
        return;
    }

    toolBar->curTool().mousePressed(m_context);

    if (event->type() != QEvent::TabletPress) {
        contextEvent->setPressure(1.0);
    }
}

void Controller::mouseDoubleClick(QMouseEvent *event)
{
    Event *contextEvent{m_context->uiContext()->event()};
    ToolBar *toolBar{m_context->uiContext()->toolBar()};
    Canvas *canvas{m_context->renderingContext()->canvas()};

    contextEvent->setPos(event->pos(), canvas->scale());
    contextEvent->setButton(event->button());
    contextEvent->setModifiers(event->modifiers());

    toolBar->curTool().mouseDoubleClick(m_context);
}

void Controller::mouseTripleClick(QMouseEvent *event)
{
    Event *contextEvent{m_context->uiContext()->event()};
    ToolBar *toolBar{m_context->uiContext()->toolBar()};
    Canvas *canvas{m_context->renderingContext()->canvas()};

    contextEvent->setPos(event->pos(), canvas->scale());
    contextEvent->setButton(event->button());
    contextEvent->setModifiers(event->modifiers());

    toolBar->curTool().mouseTripleClick(m_context);
}

void Controller::mouseMoved(QMouseEvent *event)
{
    m_mouseMoved = true;

    Event *contextEvent{m_context->uiContext()->event()};
    ToolBar *toolBar{m_context->uiContext()->toolBar()};
    Canvas *canvas{m_context->renderingContext()->canvas()};

    contextEvent->setPos(event->pos(), canvas->scale());
    contextEvent->setButton(event->button());
    contextEvent->setModifiers(event->modifiers());

    if (m_movingWithMiddleClick) {
        toolBar->tool(Tool::Move).mouseMoved(m_context);
        return;
    }

    toolBar->curTool().mouseMoved(m_context);
}

void Controller::mouseReleased(QMouseEvent *event)
{
    Event *contextEvent{m_context->uiContext()->event()};
    ToolBar *toolBar{m_context->uiContext()->toolBar()};
    Canvas *canvas{m_context->renderingContext()->canvas()};

    contextEvent->setPos(event->pos(), canvas->scale());
    contextEvent->setButton(event->button());
    contextEvent->setModifiers(event->modifiers());

    if (event->button() == Qt::MiddleButton) {
        m_movingWithMiddleClick = false;
        toolBar->tool(Tool::Move).mouseReleased(m_context);
        canvas->setCursor(toolBar->curTool().cursor());
        return;
    }

    toolBar->curTool().mouseReleased(m_context);
}

void Controller::tablet(QTabletEvent *event)
{
    Event *ev{m_context->uiContext()->event()};

    // TODO: Remove magic numbers
    ev->setPressure(event->pressure() / 1.60 + 0.375);
}

void Controller::keyPressed(QKeyEvent *event)
{
    Event *contextEvent{m_context->uiContext()->event()};
    ToolBar *toolBar{m_context->uiContext()->toolBar()};

    contextEvent->setKey(event->key());
    contextEvent->setModifiers(event->modifiers());
    contextEvent->setText(event->text());

    toolBar->curTool().keyPressed(m_context);
}

void Controller::keyReleased(QKeyEvent *event)
{
    Event *contextEvent{m_context->uiContext()->event()};
    ToolBar *toolBar{m_context->uiContext()->toolBar()};

    contextEvent->setKey(event->key());
    contextEvent->setModifiers(event->modifiers());
    contextEvent->setText(event->text());

    toolBar->curTool().keyReleased(m_context);
}

void Controller::inputMethodInvoked([[maybe_unused]] QInputMethodEvent *event)
{
}

void Controller::leave([[maybe_unused]] QEvent *event)
{
    ToolBar *toolBar{m_context->uiContext()->toolBar()};

    toolBar->curTool().leave(m_context);
}

void Controller::wheel(QWheelEvent *event)
{
    auto transformer{m_context->spatialContext()->coordinateTransformer()};
    const QPointF &offsetPos{m_context->spatialContext()->offsetPos()};
    const qreal zoomFactor{m_context->renderingContext()->zoomFactor()};
    Canvas *canvas{m_context->renderingContext()->canvas()};
    Event *contextEvent{m_context->uiContext()->event()};

    contextEvent->setPos(event->position().toPoint(), canvas->scale());
    contextEvent->setModifiers(event->modifiers());

    if (event->modifiers() & Qt::ControlModifier) {
        int delta{event->angleDelta().y() > 0 ? 1 : -1};
        m_context->renderingContext()->updateZoomFactor(delta, transformer->viewToWorld(contextEvent->pos()));
        return;
    }

    m_context->spatialContext()->setOffsetPos(offsetPos - event->angleDelta() / zoomFactor);

    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

#include "moc_controller.cpp"
