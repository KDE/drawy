/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <QObject>

class ArrowButtonActionsWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ArrowButtonActionsWidgetTest(QObject *parent = nullptr);
    ~ArrowButtonActionsWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
