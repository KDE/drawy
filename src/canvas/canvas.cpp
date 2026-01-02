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

    m_canvas = new QPixmap(m_sizeHint);
    m_overlay = new QPixmap(m_sizeHint);

    setBg(QColor{18, 18, 18});

    setTabletTracking(true);
    setMouseTracking(true);
    setAttribute(Qt::WA_InputMethodEnabled);

    setFocusPolicy(Qt::ClickFocus);
}

Canvas::~Canvas()
{
    Q_EMIT destroyed();

    delete m_canvas;
    delete m_overlay;
}

QSize Canvas::sizeHint() const
{
    return m_sizeHint;
}

QPixmap *Canvas::canvas() const
{
    return m_canvas;
}

QPixmap *Canvas::overlay() const
{
    return m_overlay;
}

QPixmap *Canvas::widget() const
{
    return m_widget;
}

QColor Canvas::bg() const
{
    return m_bg;
};

void Canvas::setBg(const QColor &color, QPixmap *canvas, QPixmap *overlay)
{
    m_bg = color;
    if (canvas)
        canvas->fill(color);
    else
        m_canvas->fill(color);

    if (overlay)
        overlay->fill(Qt::transparent);
    else
        m_overlay->fill(Qt::transparent);
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
void Canvas::paintEvent(QPaintEvent *event)
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
bool operator<=(const QSize &a, const QSize &b)
{
    return a.height() <= b.height() && a.width() <= b.width();
}

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
};

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    Q_EMIT mouseReleased(event);
    QWidget::mouseReleaseEvent(event);
};

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
        if (ev && (ev->key() == Qt::Key_Tab) || (ev->key() == Qt::Key_Backtab)) {
            Q_EMIT keyPressed(ev);
            return true;
        }
    }
    return QWidget::event(event);
}

// PRIVATE
QByteArray Canvas::imageData(QPixmap *const img)
{
    QByteArray arr{};
    QBuffer buffer{&arr};
    buffer.open(QBuffer::WriteOnly);
    img->save(&buffer, "PNG");
    return arr;
}

void Canvas::setImageData(QPixmap *const img, const QByteArray &arr)
{
    img->loadFromData(arr, "PNG");
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

    QPixmap *canvas{new QPixmap(m_maxSize)};
    QPixmap *overlay{new QPixmap(m_maxSize)};
    setBg(bg(), canvas, overlay);

    QPainter canvasPainter{canvas}, overlayPainter{overlay};
    canvasPainter.drawPixmap(0, 0, *m_canvas);
    overlayPainter.drawPixmap(0, 0, *m_overlay);

    delete m_canvas;
    delete m_overlay;

    m_canvas = canvas;
    m_overlay = overlay;

    canvasPainter.end();
    overlayPainter.end();
    Q_EMIT resizeEnd();
}

void Canvas::triggerUpdate()
{
    this->update();
}

#include "moc_canvas.cpp"
