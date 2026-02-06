// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawywidgets_private_export.h"
#include "tools/tool.hpp"
class SelectionToolState;

class LIBDRAWYWIDGETS_TESTS_EXPORT SelectionTool : public Tool
{
public:
    explicit SelectionTool(ApplicationContext *context);
    ~SelectionTool() override = default;

    [[nodiscard]] QString tooltip() const override;
    [[nodiscard]] QString icon() const override;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void keyPressed(ApplicationContext *context) override;

    [[nodiscard]] QList<Property::Type> properties() const override;

    [[nodiscard]] Tool::Type type() const override;

private:
    std::shared_ptr<SelectionToolState> getCurrentState(ApplicationContext *context);

    std::shared_ptr<SelectionToolState> m_moveState;
    std::shared_ptr<SelectionToolState> m_selectState;
    std::shared_ptr<SelectionToolState> m_curState;

    bool m_stateLocked{false};
};
