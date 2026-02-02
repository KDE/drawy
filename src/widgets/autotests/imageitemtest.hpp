/*
 * SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QObject>

class ImageItemTest : public QObject
{
    Q_OBJECT
public:
    explicit ImageItemTest(QObject *parent = nullptr);
    ~ImageItemTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldSerializeDefaultValue();
    void shouldSerializeImage();
};
