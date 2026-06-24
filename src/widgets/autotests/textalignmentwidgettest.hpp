/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include <QObject>

class TextAlignmentWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit TextAlignmentWidgetTest(QObject *parent = nullptr);
    ~TextAlignmentWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
