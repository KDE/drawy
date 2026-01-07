/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <QObject>

class PropertyTest : public QObject
{
    Q_OBJECT
public:
    explicit PropertyTest(QObject *parent = nullptr);
    ~PropertyTest() override = default;

private Q_SLOTS:
    void shouldConvertStringToEnum();
    void shouldConvertEnumToString();
};
