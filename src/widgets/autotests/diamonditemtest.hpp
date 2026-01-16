/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <QObject>

class DiamondItemTest : public QObject
{
    Q_OBJECT
public:
    explicit DiamondItemTest(QObject *parent = nullptr);
    ~DiamondItemTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldSerializeDefaultValue();
};
