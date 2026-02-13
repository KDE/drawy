/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class StandardPluginUtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit StandardPluginUtilsTest(QObject *parent = nullptr);
    ~StandardPluginUtilsTest() override = default;

private Q_SLOTS:
    void shouldHavePluginName();
    void shouldConvertStandardFormPluginTypeToString();
    void shouldConvertStandardFormPluginTypeFromString();
};
