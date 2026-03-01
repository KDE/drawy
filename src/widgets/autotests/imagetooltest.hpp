/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class ImageToolTest : public QObject
{
    Q_OBJECT
public:
    explicit ImageToolTest(QObject *parent = nullptr);
    ~ImageToolTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
