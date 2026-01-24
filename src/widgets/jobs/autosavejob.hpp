/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "libdrawywidgets_private_export.h"
#include <QObject>

class ApplicationContext;
class LIBDRAWYWIDGETS_TESTS_EXPORT AutoSaveJob : public QObject
{
    Q_OBJECT
public:
    explicit AutoSaveJob(ApplicationContext *context, QObject *parent = nullptr);
    ~AutoSaveJob() override;

    void start();

private:
    LIBDRAWYWIDGETS_NO_EXPORT void saveFile();
    ApplicationContext *const m_context;
};
