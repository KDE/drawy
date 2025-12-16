/*
 * Drawy - A simple brainstorming tool with an infinite canvas
 * Copyright (C) 2025 - Prayag Jain <prayagjain2@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
