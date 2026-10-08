/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_private_export.h"
#include "loadjobutil.hpp"
#include <QObject>
class ApplicationContext;
class LIBDRAWYWIDGETS_TESTS_EXPORT LoadJob : public QObject
{
    Q_OBJECT
public:
    explicit LoadJob(ApplicationContext *context, QObject *parent = nullptr);
    ~LoadJob() override;

    [[nodiscard]] bool canStart() const;

    void start();

    [[nodiscard]] QString fileName() const;
    void setFileName(const QString &newFileName);
    void setIsAutoSave(bool value);

Q_SIGNALS:
    void loadDone(const LoadJobUtil::DeserializeInfo &info);
    void loadFailed();

private:
    LIBDRAWYWIDGETS_NO_EXPORT void slotDeserializeDone(const LoadJobUtil::DeserializeInfo &info);
    LIBDRAWYWIDGETS_NO_EXPORT void slotDeserializeFailed();
    QString mFileName;
    bool mIsAutoSave{false};
    ApplicationContext *const mApplicationContext;
};
