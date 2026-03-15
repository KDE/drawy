// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <deque>
#include <memory>

#include "itemcommand.hpp"

#include "libdrawywidgets_private_export.h"

class ApplicationContext;
class LIBDRAWYWIDGETS_TESTS_EXPORT CommandHistory : public QObject
{
    Q_OBJECT
public:
    explicit CommandHistory(ApplicationContext *context, QObject *parent = nullptr);
    ~CommandHistory() override;

    void undo();
    void redo();
    void insert(const std::shared_ptr<ItemCommand> &command);

    void clear();

    [[nodiscard]] bool canUndo() const;
    [[nodiscard]] bool canRedo() const;

    void setUndoLimit(int limit);
Q_SIGNALS:
    void undoRedoChanged();
    void redoTextChanged(const QString &redoText);
    void undoTextChanged(const QString &undoText);

private:
    void updateUndoRedoActions();
    quint32 m_maxCommands = 100;
    std::unique_ptr<std::deque<std::shared_ptr<ItemCommand>>> m_undoStack;
    std::unique_ptr<std::deque<std::shared_ptr<ItemCommand>>> m_redoStack;

    ApplicationContext *const m_context;
};
