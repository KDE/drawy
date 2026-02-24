/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class PluginFormUtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit PluginFormUtilsTest(QObject *parent = nullptr);
    ~PluginFormUtilsTest() override = default;
private Q_SLOTS:
    void shouldDefaultValues();
};
