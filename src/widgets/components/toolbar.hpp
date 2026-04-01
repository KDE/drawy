// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "context/applicationcontext.hpp"
#include "frame.hpp"
#include <QShowEvent>

#include "tools/customtool.hpp"
#include "tools/tool.hpp"
class QButtonGroup;
class QHBoxLayout;

class ToolBar : public Frame
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);
    ~ToolBar() override;

    [[nodiscard]] Tool &curTool() const;
    void addTool(const std::shared_ptr<Tool> &tool, Tool::Type type, const QString &name);

    [[nodiscard]] Tool &tool(Tool::Type type) const;
    void changeTool(Tool::Type type);

    void showEvent(QShowEvent *event) override;

    void addCustomTool(const std::shared_ptr<CustomTool> &tool);
    void addImageTool(const std::shared_ptr<Tool> &tool);

Q_SIGNALS:
    void toolChanged(Tool &);
    void toolbarShown();

private:
    void onToolChanged(int id);
    QButtonGroup *const m_group;
    QHBoxLayout *const m_layout;
    std::unordered_map<Tool::Type, std::shared_ptr<Tool>> m_tools{};
};
