// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QCursor>
#include <QPointF>

#include "selectiontoolstate.hpp"

class SelectionToolRotateState : public SelectionToolState
{
public:
    SelectionToolRotateState();

    [[nodiscard]] bool mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    [[nodiscard]] bool mouseReleased(ApplicationContext *context) override;

private:
    QPointF m_lastPoint{};
    QPointF m_centerPoint{};
    QCursor m_cursor{};
};
