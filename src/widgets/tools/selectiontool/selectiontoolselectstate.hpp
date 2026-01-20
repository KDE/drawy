// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QPointF>

#include "selectiontoolstate.hpp"

class SelectionToolSelectState : public SelectionToolState
{
public:
    [[nodiscard]] bool mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    [[nodiscard]] bool mouseReleased(ApplicationContext *context) override;

private:
    QPointF m_lastPos;
};
