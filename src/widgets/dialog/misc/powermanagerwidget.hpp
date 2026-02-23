/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "dialog/configgroupinterface.hpp"
#include "libdrawywidgets_private_export.h"
#include <QWidget>
class QCheckBox;
class LIBDRAWYWIDGETS_TESTS_EXPORT PowerManagerWidget : public ConfigGroupInterface
{
    Q_OBJECT
public:
    explicit PowerManagerWidget(QWidget *parent = nullptr);
    ~PowerManagerWidget() override;

    void save() override;
    void load() override;

private:
    QCheckBox *const mEnabled;
};
