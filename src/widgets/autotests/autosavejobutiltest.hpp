/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class AutoSaveJobUtilTest : public QObject
{
    Q_OBJECT
public:
    explicit AutoSaveJobUtilTest(QObject *parent = nullptr);
    ~AutoSaveJobUtilTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultFileName();
};
