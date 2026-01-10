// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QTimer>
#include <QWidget>
class Canvas;
class ApplicationContext;

class RenderingContext : public QObject
{
    Q_OBJECT

public:
    explicit RenderingContext(ApplicationContext *context);
    ~RenderingContext() override;

    void setRenderingContext();

    Canvas &canvas() const;

    void markForRender();
    void markForUpdate();
    void markForUpdate(const QRect &region);

    qreal zoomFactor() const;
    void setZoomFactor(qreal newValue);
    void updateZoomFactor(qreal diff, QPoint center = {-1, -1});

    int fps() const;

    void reset();

private:
    void canvasResized();
    Canvas *m_canvas{nullptr};

    QTimer m_frameTimer;

    bool m_needsReRender{false};
    bool m_needsUpdate{false};
    QRect m_updateRegion{};

    qreal m_zoomFactor{1};

    ApplicationContext *const m_applicationContext;
};
