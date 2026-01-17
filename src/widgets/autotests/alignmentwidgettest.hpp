/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class AlignmentWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit AlignmentWidgetTest(QObject *parent = nullptr);
    ~AlignmentWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
