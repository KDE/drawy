/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class ConfigureGeneralWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ConfigureGeneralWidgetTest(QObject *parent = nullptr);
    ~ConfigureGeneralWidgetTest() override = default;
};
