/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QObject>

class ManagePowerManagement : public QObject
{
    Q_OBJECT
public:
    explicit ManagePowerManagement(QObject *parent = nullptr);
    ~ManagePowerManagement() override;

    void inhibitPowerManagement();
    void allowPowerManagement();

private:
    uint mScreenInhibitCookie = 0;
    int mSleepInhibitFd = -1;
};
