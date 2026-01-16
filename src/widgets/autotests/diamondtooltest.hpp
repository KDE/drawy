/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class DiamondToolTest : public QObject
{
    Q_OBJECT
public:
    explicit DiamondToolTest(QObject *parent = nullptr);
    ~DiamondToolTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
