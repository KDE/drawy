/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class CommandHistoryTest : public QObject
{
    Q_OBJECT
public:
    explicit CommandHistoryTest(QObject *parent = nullptr);
    ~CommandHistoryTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldTestInsertCommands();
    void shouldTestUndoCommands();
    void shouldTestClearCommands();
};
