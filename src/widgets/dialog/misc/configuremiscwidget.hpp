/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "libdrawywidgets_private_export.h"
#include <QWidget>
class PowerManagerWidget;
class LIBDRAWYWIDGETS_TESTS_EXPORT ConfigureMiscWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureMiscWidget(QWidget *parent = nullptr);
    ~ConfigureMiscWidget() override;

    void save();
    void load();
    void restoreToDefaults();

private:
    PowerManagerWidget *const mPowerManagerWidget;
};
