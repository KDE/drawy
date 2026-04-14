// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "canvas.hpp"

#include <QResizeEvent>
#include <QScreen>

Canvas::Canvas(QWidget *parent)
    : QWidget{parent}
    , m_maxSize(m_sizeHint)
{
    m_sizeHint = screen()->size() * m_scale;

    m_canvas = std::make_unique<QPixmap>(m_sizeHint);
    m_overlay = std::make_unique<QPixmap>(m_sizeHint);

    setCanvasBg(QColor{18, 18, 18});
    setOverlayBg(Qt::transparent);

    setTabletTracking(true);
    setMouseTracking(true);
    setAttribute(Qt::WA_InputMethodEnabled);

    setFocusPolicy(Qt::ClickFocus);
    setContextMenuPolicy(Qt::CustomContextMenu);

    setAcceptDrops(true);
}

Canvas::~Canvas() = default;

QSize Canvas::sizeHint() const
{
    return m_sizeHint;
}

QColor Canvas::canvasBg() const
{
    return m_canvasBg;
}

QColor Canvas::overlayBg() const
{
    return m_overlayBg;
}

void Canvas::setCanvasBg(const QColor &color)
{
    m_canvasBg = color;
    m_canvas->fill(color);
}

void Canvas::setOverlayBg(const QColor &color)
{
    m_overlayBg = color;
    m_overlay->fill(color);

    m_isOverlayDirty = (color != Qt::transparent);
}

void Canvas::paintCanvas(const std::function<void(QPainter &)> &paintFunc)
{
    QPainter painter{m_canvas.get()};
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    paintFunc(painter);
}

void Canvas::paintOverlay(const std::function<void(QPainter &)> &paintFunc)
{
    QPainter painter{m_overlay.get()};
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    paintFunc(painter);

    // assumes something must have been drawn on the overlay if this
    // function was used
    m_isOverlayDirty = true;
}

qreal Canvas::scale() const
{
    return m_scale;
}

void Canvas::setScale(const qreal scale)
{
    if (scale == 0 || m_scale == 0) {
        return;
    }

    m_sizeHint = (m_sizeHint * scale) / m_scale;

    if (scale > m_scale) {
        m_maxSize = m_sizeHint;
    }
    m_scale = scale;
    resize();
}

QSize Canvas::dimensions() const
{
    return size() * m_scale;
}

// PROTECTED
void Canvas::paintEvent([[maybe_unused]] QPaintEvent *event)
{
    QPainter painter{this};
    painter.scale(1.0 / m_scale, 1.0 / m_scale);

    if (m_canvas) {
        painter.setClipRegion(m_canvas->rect());
        painter.drawPixmap(0, 0, *m_canvas);
    }
    if (m_overlay) {
        painter.drawPixmap(0, 0, *m_overlay);
    }
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    Q_EMIT resizeEventCalled();

    setScale(devicePixelRatioF());
    resize();

    QWidget::resizeEvent(event);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    Q_EMIT mousePressed(event);
    QWidget::mousePressEvent(event);
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    Q_EMIT mouseMoved(event);
    QWidget::mouseMoveEvent(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    Q_EMIT mouseReleased(event);
    QWidget::mouseReleaseEvent(event);
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    Q_EMIT keyPressed(event);
    QWidget::keyPressEvent(event);
}

void Canvas::keyReleaseEvent(QKeyEvent *event)
{
    Q_EMIT keyReleased(event);
    QWidget::keyReleaseEvent(event);
}

void Canvas::inputMethodEvent(QInputMethodEvent *event)
{
    Q_EMIT inputMethodInvoked(event);
    QWidget::inputMethodEvent(event);
}

void Canvas::tabletEvent(QTabletEvent *event)
{
    Q_EMIT tablet(event);
    QWidget::tabletEvent(event);
}

void Canvas::wheelEvent(QWheelEvent *event)
{
    Q_EMIT wheel(event);
    QWidget::wheelEvent(event);
}

void Canvas::leaveEvent(QEvent *event)
{
    Q_EMIT leave(event);
    QWidget::leaveEvent(event);
}

void Canvas::dragEnterEvent(QDragEnterEvent *event)
{
    Q_EMIT dragEnter(event);
    QWidget::dragEnterEvent(event);
}

void Canvas::dragMoveEvent(QDragMoveEvent *event)
{
    Q_EMIT dragMove(event);
    QWidget::dragMoveEvent(event);
}

void Canvas::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_EMIT dragLeave(event);
    QWidget::dragLeaveEvent(event);
}

void Canvas::dropEvent(QDropEvent *event)
{
    Q_EMIT drop(event);
    QWidget::dropEvent(event);
}

bool Canvas::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ev = dynamic_cast<QKeyEvent *>(event);
        if (ev && ((ev->key() == Qt::Key_Tab) || (ev->key() == Qt::Key_Backtab))) {
            Q_EMIT keyPressed(ev);
            return true;
        }
    }
    return QWidget::event(event);
}

void Canvas::resize()
{
    if (m_canvas->paintingActive() || m_overlay->paintingActive()) {
        return;
    }

    const QSize oldSize{m_canvas->size()};
    const QSize newSize{size() * m_scale};
    m_maxSize.setWidth(std::max(oldSize.width(), newSize.width()));
    m_maxSize.setHeight(std::max(oldSize.height(), newSize.height()));

    std::unique_ptr<QPixmap> canvas{new QPixmap(m_maxSize)};
    std::unique_ptr<QPixmap> overlay{new QPixmap(m_maxSize)};

    canvas->fill(canvasBg());
    overlay->fill(overlayBg());

    QPainter canvasPainter{canvas.get()};
    QPainter overlayPainter{overlay.get()};
    canvasPainter.drawPixmap(0, 0, *m_canvas);
    overlayPainter.drawPixmap(0, 0, *m_overlay);

    m_canvas = std::move(canvas);
    m_overlay = std::move(overlay);

    canvasPainter.end();
    overlayPainter.end();
}

bool Canvas::overlayDirty() const
{
    return m_isOverlayDirty;
}

// returns a copy of the content on the canvas
QPixmap Canvas::canvasPixmap() const
{
    return QPixmap{*m_canvas};
}

QPixmap Canvas::overlayPixmap() const
{
    return QPixmap{*m_canvas};
}

#include "moc_canvas.cpp"
