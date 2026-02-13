/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class TriangleItemTest : public QObject
{
    Q_OBJECT
public:
    explicit TriangleItemTest(QObject *parent = nullptr);
    ~TriangleItemTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
