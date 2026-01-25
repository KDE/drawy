// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "tool.hpp"

class MoveTool : public Tool
{
public:
    MoveTool();
    ~MoveTool() override = default;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;

    [[nodiscard]] Tool::Type type() const override;

    [[nodiscard]] QString tooltip() const override;
    [[nodiscard]] QString icon() const override;

private:
    bool m_isActive{false};
    QPointF m_initialOffsetPos{};
    QPointF m_initialPos{};
};
