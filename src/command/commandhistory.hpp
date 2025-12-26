// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <deque>
#include <memory>

#include "command.hpp"
class ApplicationContext;

class CommandHistory {
public:
    CommandHistory(ApplicationContext *context);
    ~CommandHistory();

    void undo();
    void redo();
    void insert(const std::shared_ptr<Command>& command);

    static constexpr int maxCommands{100};  // arbitrary

    void clear();

private:
    std::unique_ptr<std::deque<std::shared_ptr<Command>>> m_undoStack;
    std::unique_ptr<std::deque<std::shared_ptr<Command>>> m_redoStack;

    ApplicationContext *m_context;
};
