/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class ArrowToolTest : public QObject
{
    Q_OBJECT
public:
    explicit ArrowToolTest(QObject *parent = nullptr);
    ~ArrowToolTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
