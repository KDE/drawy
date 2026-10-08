/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QObject>

class PageManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit PageManagerTest(QObject *parent = nullptr);
    ~PageManagerTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldInsertPages();
    void shouldNotInsertInvalidIndex();
    void shouldRemoveCurrentPage();
    void shouldShiftCurrentPageOnRemove();
    void shouldShiftCurrentPageOnInsert();
    void shouldRemoveLastPage();
    void shouldNotSetInvalidCurrentPage();
    void shouldRenamePage();
    void shouldSetPages();
    void shouldMovePage();
};
