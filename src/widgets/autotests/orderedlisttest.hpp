/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class OrderedListTest : public QObject
{
    Q_OBJECT
public:
    explicit OrderedListTest(QObject *parent = nullptr);
    ~OrderedListTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldContainsItems();
    void shouldRemoveItems();
    void shouldAddItems();
    void shouldUpdateList();
    void shouldCheckMove();
    void shouldClearList();
};
