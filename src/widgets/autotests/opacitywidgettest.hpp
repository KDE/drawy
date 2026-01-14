/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class OpacityWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit OpacityWidgetTest(QObject *parent = nullptr);
    ~OpacityWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
