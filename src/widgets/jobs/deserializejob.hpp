/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_private_export.h"
#include <QJsonObject>
#include <QObject>
class LIBDRAWYWIDGETS_TESTS_EXPORT DeserializeJob : public QObject
{
    Q_OBJECT
public:
    explicit DeserializeJob(QObject *parent = nullptr);
    ~DeserializeJob() override;

    [[nodiscard]] bool canStart() const;

    void start();
    [[nodiscard]] QJsonObject jsonObject() const;
    void setJsonObject(const QJsonObject &newJsonObject);

Q_SIGNALS:
    void deserializeDone();

private:
    LIBDRAWYWIDGETS_NO_EXPORT void deserializeItems();
    QJsonObject mJsonObject;
};
