// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QButtonGroup>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>

#include "../tools/tool.hpp"

class ToolBar : public QFrame {
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);
    ~ToolBar() override;

    Tool &curTool() const;
    void addTool(const std::shared_ptr<Tool>& tool, Tool::Type type);
    QVector<std::shared_ptr<Tool>> tools() const;

    Tool &tool(Tool::Type type) const;
    void changeTool(Tool::Type type);

signals:
    void toolChanged(Tool &);

private:
    QButtonGroup *m_group{};
    QHBoxLayout *m_layout{};
    std::unordered_map<int, std::shared_ptr<Tool>> m_tools{};
    void createButtons() const;

private slots:
    void onToolChanged(int id);
};
