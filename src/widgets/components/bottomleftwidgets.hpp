// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QHBoxLayout>
#include <QWidget>
class ActionManager;
class BottomLeftWidgets : public QWidget
{
    Q_OBJECT
public:
    explicit BottomLeftWidgets(ActionManager *actionManager, QWidget *parent = nullptr);

Q_SIGNALS:
    void resetZoom();
    void zoomFactorChanged(qreal newZoomFactor);

private:
    QHBoxLayout *const m_layout;
};
