// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "drawingtool.hpp"
class FreeformItem;

class FreeformTool : public DrawingTool
{
public:
    FreeformTool();
    ~FreeformTool() override = default;

    [[nodiscard]] QString tooltip() const override;
    [[nodiscard]] IconManager::Icon icon() const override;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void cleanup() override;

    [[nodiscard]] Tool::Type type() const override;

private:
    std::shared_ptr<FreeformItem> curItem{};
    QPointF m_lastPoint{};
};
