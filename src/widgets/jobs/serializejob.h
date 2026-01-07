/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class SerializeJob : public QObject
{
    Q_OBJECT
public:
    explicit SerializeJob(QObject *parent = nullptr);
    ~SerializeJob() override;

    [[nodiscard]] bool canStart() const;

    void start();
};
