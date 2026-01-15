/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class EllipseToolTest : public QObject
{
    Q_OBJECT
public:
    explicit EllipseToolTest(QObject *parent = nullptr);
    ~EllipseToolTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
