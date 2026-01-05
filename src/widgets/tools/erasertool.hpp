// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <unordered_set>

#include "tool.hpp"
class Item;

class EraserTool : public Tool
{
public:
    EraserTool();
    ~EraserTool() override = default;

    [[nodiscard]] QString tooltip() const override;
    [[nodiscard]] IconManager::Icon icon() const override;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void leave(ApplicationContext *context) override;

    void cleanup() override;

    [[nodiscard]] Tool::Type type() const override;

private:
    bool m_isErasing{false};
    QRectF m_lastRect{};

    std::unordered_set<std::shared_ptr<Item>> m_toBeErased;
};
