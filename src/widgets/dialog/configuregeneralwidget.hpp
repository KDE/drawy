/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "libdrawywidgets_private_export.h"
#include <QWidget>
class QSpinBox;
class LIBDRAWYWIDGETS_TESTS_EXPORT ConfigureGeneralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureGeneralWidget(QWidget *parent = nullptr);
    ~ConfigureGeneralWidget() override;

    void save();
    void load();
    void restoreToDefaults();

private:
    QSpinBox *const mAutoSave;
};
