/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "libdrawywidgets_private_export.h"
#include <KPageDialog>
class ConfigureGeneralWidget;
class LIBDRAWYWIDGETS_TESTS_EXPORT ConfigureSettingsDialog : public KPageDialog
{
    Q_OBJECT
public:
    explicit ConfigureSettingsDialog(QWidget *parent = nullptr);
    ~ConfigureSettingsDialog() override;

private:
    LIBDRAWYWIDGETS_NO_EXPORT void readConfig();
    LIBDRAWYWIDGETS_NO_EXPORT void writeConfig();
    LIBDRAWYWIDGETS_NO_EXPORT void slotAccepted();
    LIBDRAWYWIDGETS_NO_EXPORT void load();
    LIBDRAWYWIDGETS_NO_EXPORT void slotRestoreDefaults();
    ConfigureGeneralWidget *const mConfigureGeneralWidget;
    KPageWidgetItem *mConfigureGeneralWidgetPage = nullptr;
};
