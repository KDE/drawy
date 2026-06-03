// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "drawingtool.hpp"
#include "libdrawywidgets_private_export.h"
class LineItem;

class LIBDRAWYWIDGETS_TESTS_EXPORT LineDrawingTool : public DrawingTool
{
protected:
    std::shared_ptr<LineItem> curItem;

public:
    explicit LineDrawingTool(ApplicationContext *context);
    ~LineDrawingTool() override = default;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void cleanup() override;

private:
    bool m_endSet{false};
};
