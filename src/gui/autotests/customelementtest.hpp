/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QObject>

class CustomElementTest : public QObject
{
    Q_OBJECT
public:
    explicit CustomElementTest(QObject *parent = nullptr);
    ~CustomElementTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
