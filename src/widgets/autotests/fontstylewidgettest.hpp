/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include <QObject>

class FontStyleWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit FontStyleWidgetTest(QObject *parent = nullptr);
    ~FontStyleWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
