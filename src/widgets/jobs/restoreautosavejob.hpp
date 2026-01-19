/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_private_export.h"
#include <QObject>
class ApplicationContext;
class LIBDRAWYWIDGETS_TESTS_EXPORT RestoreAutoSaveJob : public QObject
{
    Q_OBJECT
public:
    explicit RestoreAutoSaveJob(ApplicationContext *context, QObject *parent = nullptr);
    ~RestoreAutoSaveJob() override;

    void start();

    [[nodiscard]] QWidget *parentWidget() const;
    void setParentWidget(QWidget *newParentWidget);

Q_SIGNALS:
    void restoreDone();

private:
    LIBDRAWYWIDGETS_NO_EXPORT void removeAutoSaveFile();
    LIBDRAWYWIDGETS_NO_EXPORT void restoreFile();
    QWidget *mParentWidget = nullptr;
    ApplicationContext *const m_context;
};
