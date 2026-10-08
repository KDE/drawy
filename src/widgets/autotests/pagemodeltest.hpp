/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QObject>

class PageModelTest : public QObject
{
    Q_OBJECT
public:
    explicit PageModelTest(QObject *parent = nullptr);
    ~PageModelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldFollowPageManager();
    void shouldRenamePage();
    void shouldMoveRows();
};
