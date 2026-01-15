/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class RectangleToolTest : public QObject
{
    Q_OBJECT
public:
    explicit RectangleToolTest(QObject *parent = nullptr);
    ~RectangleToolTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
