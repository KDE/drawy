/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <QObject>

class ListStyleWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ListStyleWidgetTest(QObject *parent = nullptr);
    ~ListStyleWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
