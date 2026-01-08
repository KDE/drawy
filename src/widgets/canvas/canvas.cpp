// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "canvas.hpp"

#include <QBuffer>
#include <QResizeEvent>
#include <QScreen>
#include <qopenglpaintdevice.h>

// PUBLIC
Canvas::Canvas(QWidget *parent)
    : QOpenGLWidget{parent}
    , m_maxSize(m_sizeHint)
{
    m_sizeHint = screen()->size() * m_scale;

    // m_canvas = new QPixmap(m_sizeHint);
    // m_overlay = new QPixmap(m_sizeHint);

    // setCanvasBg(QColor{18, 18, 18});
    // setOverlayBg(Qt::transparent);

    setTabletTracking(true);
    setMouseTracking(true);
    setAttribute(Qt::WA_InputMethodEnabled);

    setFocusPolicy(Qt::ClickFocus);
    qDebug() << "CONSTRUCTOR";
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

QOpenGLFramebufferObject *Canvas::canvas() const
{
    return m_canvas;
}

QOpenGLFramebufferObject *Canvas::overlay() const
{
    return m_overlay;
}

QColor Canvas::canvasBg() const
{
    return m_canvasBg;
};

QColor Canvas::overlayBg() const
{
    return m_overlayBg;
};

void Canvas::setCanvasBg(const QColor &color)
{
    m_canvasBg = color;

    makeCurrent();

    m_canvas->bind();
    glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_canvas->release();

    update();
}

void Canvas::setOverlayBg(const QColor &color)
{
    m_overlayBg = color;

    makeCurrent();

    m_overlay->bind();
    glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_overlay->release();

    update();
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
// void Canvas::paintEvent([[maybe_unused]] QPaintEvent *event)
// {
//     QPainter painter{this};
//     painter.scale(1.0 / m_scale, 1.0 / m_scale);
//
//     if (m_canvas) {
//         painter.setClipRegion(m_canvas->rect());
//         painter.drawPixmap(0, 0, *m_canvas);
//     }
//     if (m_overlay)
//         painter.drawPixmap(0, 0, *m_overlay);
// }

// void Canvas::resizeEvent(QResizeEvent *event)
// {
//     Q_EMIT resizeEventCalled();
//
//     setScale(devicePixelRatioF());
//     if (size() * m_scale <= m_maxSize) {
//         return;
//     }
//
//     resize();
//
//     QWidget::resizeEvent(event);
// }

void Canvas::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(canvasBg().redF(), canvasBg().greenF(), canvasBg().blueF(), 1.0f);

    m_canvas = new QOpenGLFramebufferObject(width(), height(), QOpenGLFramebufferObject::CombinedDepthStencil);
    m_overlay = new QOpenGLFramebufferObject(width(), height(), QOpenGLFramebufferObject::CombinedDepthStencil);
}

void Canvas::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    resize();
    update();
}

void Canvas::paintGL()
{
    QPainter painter{this};
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    painter.scale(1.0 / m_scale, 1.0 / m_scale);

    if (m_canvas) {
        // painter.setClipRegion(m_canvas->rect());
        painter.drawImage(0, 0, m_canvas->toImage());
    }
    if (m_overlay)
        painter.drawImage(0, 0, m_overlay->toImage());
}

// just a small overload
bool operator<=(const QSize &a, const QSize &b)
{
    return a.height() <= b.height() && a.width() <= b.width();
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

void Canvas::paintCanvas(const std::function<void(QPainter &)> &paintFunc)
{
    makeCurrent();
    {
        m_canvas->bind();

        QOpenGLPaintDevice paintDevice{size()};
        QPainter painter{&paintDevice};
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

        paintFunc(painter);

        m_canvas->release();
    }
};

void Canvas::paintOverlay(const std::function<void(QPainter &)> &paintFunc)
{
    makeCurrent();
    {
        m_overlay->bind();

        QOpenGLPaintDevice paintDevice{size()};
        QPainter painter{&paintDevice};
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

        paintFunc(painter);

        m_overlay->release();
    }
};

// PRIVATE
// QByteArray Canvas::imageData(QPixmap *const img)
// {
//     QByteArray arr{};
//     QBuffer buffer{&arr};
//     buffer.open(QBuffer::WriteOnly);
//     img->save(&buffer, "PNG");
//     return arr;
// }
//
// void Canvas::setImageData(QPixmap *const img, const QByteArray &arr)
// {
//     img->loadFromData(arr, "PNG");
// }

void Canvas::resize()
{
    Q_EMIT resizeStart();

    QSize oldSize{m_canvas->size()};
    QSize newSize{size() * m_scale};
    m_maxSize.setWidth(std::max(oldSize.width(), newSize.width()));
    m_maxSize.setHeight(std::max(oldSize.height(), newSize.height()));

    QOpenGLFramebufferObject *canvas{new QOpenGLFramebufferObject(m_maxSize, QOpenGLFramebufferObject::CombinedDepthStencil)};
    QOpenGLFramebufferObject *overlay{new QOpenGLFramebufferObject(m_maxSize, QOpenGLFramebufferObject::CombinedDepthStencil)};

    setCanvasBg(canvasBg());
    setOverlayBg(overlayBg());

    makeCurrent();
    canvas->bind();
    QOpenGLPaintDevice canvasDevice{m_maxSize};
    QPainter canvasPainter{&canvasDevice};
    canvasPainter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    canvasPainter.drawImage(0, 0, m_canvas->toImage());
    canvas->release();

    overlay->bind();
    QOpenGLPaintDevice overlayDevice{m_maxSize};
    QPainter overlayPainter{&overlayDevice};
    overlayPainter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    overlayPainter.drawImage(0, 0, m_overlay->toImage());
    overlay->release();

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
