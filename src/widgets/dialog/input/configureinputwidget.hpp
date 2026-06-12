/*
 * SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "libdrawywidgets_private_export.h"
#include <QWidget>

class FreehandSettingsWidget;
class LIBDRAWYWIDGETS_TESTS_EXPORT ConfigureInputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureInputWidget(QWidget *parent = nullptr);
    ~ConfigureInputWidget() override;

    void save();
    void load();
    void restoreToDefaults();

private:
    FreehandSettingsWidget *m_freehandSettingsWidget;
};
