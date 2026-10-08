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
class ApplicationContext;
class LIBDRAWYWIDGETS_TESTS_EXPORT SaveAsJob : public QObject
{
    Q_OBJECT
public:
    struct SaveAsInfo {
        QString filePath;
        QList<Page *> pages;
        int currentPage = 0;
        bool isAutoSave{false};
    };

    explicit SaveAsJob(ApplicationContext *context, QObject *parent = nullptr);
    ~SaveAsJob() override;

    [[nodiscard]] bool canStart() const;

    void start();

    [[nodiscard]] SaveAsInfo saveAsInfo() const;
    void setSaveAsInfo(const SaveAsInfo &newSaveAsInfo);

Q_SIGNALS:
    void saveFileDone(const QJsonObject &obj);

private:
    LIBDRAWYWIDGETS_NO_EXPORT void slotSerializeDone(const QJsonObject &obj);
    SaveAsInfo mSaveAsInfo;
    ApplicationContext *const mApplicationContext;
};
LIBDRAWYWIDGETS_EXPORT QDebug operator<<(QDebug d, const SaveAsJob::SaveAsInfo &t);
