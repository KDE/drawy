// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

class AutoSaveWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit AutoSaveWidgetTest(QObject *parent = nullptr);
    ~AutoSaveWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
