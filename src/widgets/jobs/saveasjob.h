/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class SaveAsJob : public QObject
{
    Q_OBJECT
public:
    explicit SaveAsJob(QObject *parent = nullptr);
    ~SaveAsJob() override;

    [[nodiscard]] bool canStart() const;

    void start();
};
