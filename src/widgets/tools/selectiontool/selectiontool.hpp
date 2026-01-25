// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "tools/tool.hpp"
class SelectionToolState;

class SelectionTool : public Tool
{
public:
    SelectionTool();
    ~SelectionTool() override = default;

    [[nodiscard]] QString tooltip() const override;
    [[nodiscard]] QString icon() const override;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void keyPressed(ApplicationContext *context) override;

    const QList<Property::Type> properties() const override;

    [[nodiscard]] Tool::Type type() const override;

private:
    std::shared_ptr<SelectionToolState> getCurrentState(ApplicationContext *context);

    std::shared_ptr<SelectionToolState> m_moveState;
    std::shared_ptr<SelectionToolState> m_selectState;
    std::shared_ptr<SelectionToolState> m_curState;

    bool m_stateLocked{false};
};
