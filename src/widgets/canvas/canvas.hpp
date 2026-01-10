// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QPainter>
#include <QWidget>

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas() override;

    [[nodiscard]] QSize sizeHint() const override;
    [[nodiscard]] QSize dimensions() const;

    [[nodiscard]] QColor canvasBg() const;
    [[nodiscard]] QColor overlayBg() const;
    void setCanvasBg(const QColor &color);
    void setOverlayBg(const QColor &color);

    [[nodiscard]] qreal scale() const;
    void setScale(const qreal scale);

    void paintCanvas(const std::function<void(QPainter &)> &paintFunc);
    void paintOverlay(const std::function<void(QPainter &)> &paintFunc);

Q_SIGNALS:
    void mousePressed(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void keyPressed(QKeyEvent *event);
    void keyReleased(QKeyEvent *event);
    void inputMethodInvoked(QInputMethodEvent *event);
    void tablet(QTabletEvent *event);
    void wheel(QWheelEvent *event);
    void resizeEventCalled();
    void resizeStart();
    void resizeEnd();
    void destroyed();
    void leave(QEvent *event);

public Q_SLOTS:
    void triggerUpdate();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void inputMethodEvent(QInputMethodEvent *event) override;
    void tabletEvent(QTabletEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    bool event(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    qreal m_scale{1.0}; // default scale is 1
    QPixmap *m_canvas{};
    QPixmap *m_overlay{};

    QColor m_canvasBg{};
    QColor m_overlayBg{};

    QSize m_sizeHint{500, 500};
    QSize m_maxSize{};

    void resize();
};
