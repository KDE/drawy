// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "commandhistory.hpp"
#include "drawy_debug.h"

#include <QDebug>

CommandHistory::CommandHistory(ApplicationContext *context, QObject *parent)
    : QObject(parent)
    , m_undoStack(std::make_unique<std::deque<std::shared_ptr<Command>>>())
    , m_redoStack(std::make_unique<std::deque<std::shared_ptr<Command>>>())
    , m_context{context}
{
}

CommandHistory::~CommandHistory()
{
    qCDebug(DRAWY_LOG) << "Object deleted: CommandHistory";
}

void CommandHistory::undo()
{
    if (m_undoStack->empty())
        return;

    std::shared_ptr<Command> lastCommand{m_undoStack->front()};
    lastCommand->undo(m_context);

    m_redoStack->push_front(lastCommand);

    if (m_redoStack->size() == maxCommands)
        m_redoStack->pop_back();

    m_undoStack->pop_front();
    Q_EMIT undoRedoChanged();
}

void CommandHistory::redo()
{
    if (m_redoStack->empty())
        return;

    std::shared_ptr<Command> nextCommand{m_redoStack->front()};
    nextCommand->execute(m_context);

    m_undoStack->push_front(nextCommand);
    if (m_undoStack->size() == maxCommands)
        m_undoStack->pop_back();

    m_redoStack->pop_front();
    Q_EMIT undoRedoChanged();
}

void CommandHistory::insert(const std::shared_ptr<Command> &command)
{
    while (!m_redoStack->empty()) {
        m_redoStack->pop_front();
    }

    command->execute(m_context);

    m_undoStack->push_front(command);
    if (m_undoStack->size() == maxCommands) {
        m_undoStack->pop_back();
    }
    Q_EMIT undoRedoChanged();
}

void CommandHistory::clear()
{
    m_undoStack->clear();
    m_redoStack->clear();
    Q_EMIT undoRedoChanged();
}

bool CommandHistory::hasUndo() const
{
    return !m_undoStack->empty();
}

bool CommandHistory::hasRedo() const
{
    return !m_redoStack->empty();
}
