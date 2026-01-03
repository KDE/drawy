// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QDateTime>
#include <QMouseEvent>
#include <QObject>

#include "tools/tool.hpp"
class ApplicationContext;

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller() override;

public Q_SLOTS:
    void mousePressed(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void mouseDoubleClick(QMouseEvent *event);
    void mouseTripleClick(QMouseEvent *event);
    void keyPressed(QKeyEvent *event);
    void keyReleased(QKeyEvent *event);
    void inputMethodInvoked(QInputMethodEvent *event);
    void tablet(QTabletEvent *event);
    void wheel(QWheelEvent *event);
    void leave(QEvent *event);

private:
    ApplicationContext *m_context{};
    qint64 m_lastTime{};
    qint64 m_lastClickTime{};
    int m_clickCount{}; // for double/triple clicks

    bool m_mouseMoved{false};
    bool m_movingWithMiddleClick{false};
};
