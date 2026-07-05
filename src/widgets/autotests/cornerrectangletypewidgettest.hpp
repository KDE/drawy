/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class CornerRectangleTypeWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit CornerRectangleTypeWidgetTest(QObject *parent = nullptr);
    ~CornerRectangleTypeWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
