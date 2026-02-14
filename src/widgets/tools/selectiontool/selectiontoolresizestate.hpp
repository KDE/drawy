// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QPointF>

#include "selectiontool.hpp"
#include "selectiontoolstate.hpp"

class SelectionToolResizeState : public SelectionToolState
{
public:
    [[nodiscard]] bool mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    [[nodiscard]] bool mouseReleased(ApplicationContext *context) override;

    void setHandle(SelectionTool::SelectionHandle handle);

private:
    QCursor cursorForHandle(double angle) const;

    QPointF m_viewLastPoint{};
    SelectionTool::SelectionHandle m_handle{};
};
