/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <QObject>

class PolygonItemTest : public QObject
{
    Q_OBJECT
public:
    explicit PolygonItemTest(QObject *parent = nullptr);
    ~PolygonItemTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
