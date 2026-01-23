// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "canvas.hpp"

#include <QBuffer>
#include <QResizeEvent>
#include <QScreen>

// PUBLIC
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
}

Canvas::~Canvas()
{
    Q_EMIT destroyed();
}

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
}

qreal Canvas::scale() const
{
    return m_scale;
}

void Canvas::setScale(const qreal scale)
{
    if (scale == 0 || m_scale == 0)
        return;

    m_sizeHint = (m_sizeHint * scale) / m_scale;

    if (scale > m_scale)
        m_maxSize = m_sizeHint;
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
    if (m_overlay)
        painter.drawPixmap(0, 0, *m_overlay);
}

// just a small overload
namespace
{
inline bool operator<=(const QSize &a, const QSize &b)
{
    return a.height() <= b.height() && a.width() <= b.width();
}
};

void Canvas::resizeEvent(QResizeEvent *event)
{
    Q_EMIT resizeEventCalled();

    setScale(devicePixelRatioF());
    if (size() * m_scale <= m_maxSize) {
        return;
    }

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
    Q_EMIT resizeStart();

    if (m_canvas->paintingActive() || m_overlay->paintingActive()) {
        return;
    }

    QSize oldSize{m_canvas->size()};
    QSize newSize{size() * m_scale};
    m_maxSize.setWidth(std::max(oldSize.width(), newSize.width()));
    m_maxSize.setHeight(std::max(oldSize.height(), newSize.height()));

    std::unique_ptr<QPixmap> canvas{new QPixmap(m_maxSize)};
    std::unique_ptr<QPixmap> overlay{new QPixmap(m_maxSize)};

    setCanvasBg(canvasBg());
    setOverlayBg(overlayBg());

    QPainter canvasPainter{canvas.get()}, overlayPainter{overlay.get()};
    canvasPainter.drawPixmap(0, 0, *m_canvas);
    overlayPainter.drawPixmap(0, 0, *m_overlay);

    m_canvas = std::move(canvas);
    m_overlay = std::move(overlay);

    canvasPainter.end();
    overlayPainter.end();
    Q_EMIT resizeEnd();
}

// returns a copy of the content on the canvas
QPixmap Canvas::canvasPixmap() const
{
    return QPixmap{*m_canvas};
}

void Canvas::triggerUpdate()
{
    this->update();
}

#include "moc_canvas.cpp"
