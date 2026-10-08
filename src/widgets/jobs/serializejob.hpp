/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "libdrawywidgets_private_export.h"
#include <QList>
#include <QObject>
class Page;
class QDebug;
class QJsonObject;
class ApplicationContext;
class QJsonArray;
class LIBDRAWYWIDGETS_TESTS_EXPORT SerializeJob : public QObject
{
    Q_OBJECT
public:
    struct SerializeInfo {
        int currentPage = 0;
        QList<Page *> pages;
    };

    explicit SerializeJob(ApplicationContext *context, QObject *parent = nullptr);
    ~SerializeJob() override;

    [[nodiscard]] bool canStart() const;

    void start();

    [[nodiscard]] SerializeInfo serializeInfo() const;
    void setSerializeInfo(const SerializeInfo &newSerializeInfo);

Q_SIGNALS:
    void serializeDone(const QJsonObject &obj);

private:
    LIBDRAWYWIDGETS_NO_EXPORT void serializeItems();
    [[nodiscard]] LIBDRAWYWIDGETS_NO_EXPORT QJsonArray serializePages() const;
    SerializeInfo mSerializeInfo;
    ApplicationContext *const mApplicationContext;
};
LIBDRAWYWIDGETS_EXPORT QDebug operator<<(QDebug d, const SerializeJob::SerializeInfo &t);
