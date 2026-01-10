/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "deserializejob.hpp"
#include "libdrawywidgets_private_export.h"
#include <QObject>
class LIBDRAWYWIDGETS_TESTS_EXPORT LoadJob : public QObject
{
    Q_OBJECT
public:
    struct LoadInfo {
        QPointF offsetPos;
        qreal zoomFactor = 0;
        QList<std::shared_ptr<Item>> items;
    };
    explicit LoadJob(QObject *parent = nullptr);
    ~LoadJob() override;

    [[nodiscard]] bool canStart() const;

    void start();

    [[nodiscard]] QString fileName() const;
    void setFileName(const QString &newFileName);

Q_SIGNALS:
    void loadDone(const LoadJob::LoadInfo &info);

private:
    LIBDRAWYWIDGETS_NO_EXPORT void slotDeserializeDone(const DeserializeJob::DeserializeInfo &info);
    QString mFileName;
};
