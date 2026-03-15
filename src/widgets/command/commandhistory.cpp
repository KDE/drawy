// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "commandhistory.hpp"
#include "drawy_command_debug.h"

CommandHistory::CommandHistory(ApplicationContext *context, QObject *parent)
    : QObject(parent)
    , m_undoStack(std::make_unique<std::deque<std::shared_ptr<ItemCommand>>>())
    , m_redoStack(std::make_unique<std::deque<std::shared_ptr<ItemCommand>>>())
    , m_context{context}
{
}

CommandHistory::~CommandHistory()
{
    qCDebug(DRAWY_COMMAND_LOG) << "Object deleted: CommandHistory";
}

void CommandHistory::setUndoLimit(int limit)
{
    m_maxCommands = limit;
}

void CommandHistory::undo()
{
    if (m_undoStack->empty()) {
        return;
    }

    const std::shared_ptr<ItemCommand> lastCommand{m_undoStack->front()};
    if (m_context) {
        lastCommand->undo(m_context);
    }

    m_redoStack->push_front(lastCommand);

    if (m_redoStack->size() == m_maxCommands) {
        m_redoStack->pop_back();
    }

    m_undoStack->pop_front();
    updateUndoRedoActions();
}

void CommandHistory::redo()
{
    if (m_redoStack->empty()) {
        return;
    }

    const std::shared_ptr<ItemCommand> nextCommand{m_redoStack->front()};
    if (m_context) {
        nextCommand->redo(m_context);
    }

    m_undoStack->push_front(nextCommand);
    if (m_undoStack->size() == m_maxCommands) {
        m_undoStack->pop_back();
    }

    m_redoStack->pop_front();
    updateUndoRedoActions();
}

void CommandHistory::updateUndoRedoActions()
{
    Q_EMIT undoRedoChanged();
    if (!m_undoStack->empty()) {
        Q_EMIT undoTextChanged(m_undoStack->front()->text());
    }
    if (!m_redoStack->empty()) {
        Q_EMIT redoTextChanged(m_redoStack->front()->text());
    }
}

void CommandHistory::push(const std::shared_ptr<ItemCommand> &command)
{
    qCDebug(DRAWY_COMMAND_LOG) << "Insert command:" << command->text();
    while (!m_redoStack->empty()) {
        m_redoStack->pop_front();
    }

    if (m_context) {
        command->redo(m_context);
    }

    m_undoStack->push_front(command);
    if (m_undoStack->size() == m_maxCommands) {
        m_undoStack->pop_back();
    }
    updateUndoRedoActions();
}

void CommandHistory::clear()
{
    m_undoStack->clear();
    m_redoStack->clear();
    updateUndoRedoActions();
}

bool CommandHistory::canUndo() const
{
    return !m_undoStack->empty();
}

bool CommandHistory::canRedo() const
{
    return !m_redoStack->empty();
}

#include "moc_commandhistory.cpp"
