// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
// SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "config-drawy.hpp"
#include "libdrawywidgets_export.h"
#include <QWidget>
class AutoSaveJob;
class ApplicationContext;
class ManagePowerManagement;
class LIBDRAWYWIDGETS_EXPORT MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void viewFullScreen(bool fullScreen);
    void loadFile(const QString &fileName);
    void activeDebug();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    LIBDRAWYWIDGETS_NO_EXPORT void contextMenuRequested(const QPoint &pos);
    LIBDRAWYWIDGETS_NO_EXPORT void loadCustomFonts();
    LIBDRAWYWIDGETS_NO_EXPORT void updateWindowTitle();
    bool m_forceClose = false;
    AutoSaveJob *m_autoSaveJob{nullptr};
    ApplicationContext *const mApplicationContext;
#if WITH_DBUS
    ManagePowerManagement *const mManagePowerManagement;
#endif
};
