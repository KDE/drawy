// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "drawingtool.hpp"
#include "libdrawywidgets_private_export.h"
class PolygonItem;

class LIBDRAWYWIDGETS_TESTS_EXPORT PolygonDrawingTool : public DrawingTool
{
protected:
    std::shared_ptr<PolygonItem> curItem{};

public:
    PolygonDrawingTool();
    ~PolygonDrawingTool() override = default;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void cleanup() override;

    [[nodiscard]] Tool::Type type() const override;
};
