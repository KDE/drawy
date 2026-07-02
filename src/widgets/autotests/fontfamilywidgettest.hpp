/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include <QObject>

class FontFamilyWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit FontFamilyWidgetTest(QObject *parent = nullptr);
    ~FontFamilyWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
