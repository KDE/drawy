// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QButtonGroup>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>

#include "tools/tool.hpp"

class ToolBar : public QFrame
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);
    ~ToolBar() override;

    [[nodiscard]] Tool &curTool() const;
    void addTool(const std::shared_ptr<Tool> &tool, Tool::Type type, const QString &name);

    [[nodiscard]] Tool &tool(Tool::Type type) const;
    void changeTool(Tool::Type type);

Q_SIGNALS:
    void toolChanged(Tool &);

private:
    void createButtons() const;
    void onToolChanged(int id);
    QButtonGroup *const m_group;
    QHBoxLayout *const m_layout;
    std::unordered_map<Tool::Type, std::shared_ptr<Tool>> m_tools{};
};
