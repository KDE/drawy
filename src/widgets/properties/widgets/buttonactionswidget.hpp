/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_private_export.h"
#include <QPushButton>

class LIBDRAWYWIDGETS_TESTS_EXPORT ButtonActionsWidget : public QPushButton
{
    Q_OBJECT
public:
    explicit ButtonActionsWidget(QWidget *parent = nullptr);
    ~ButtonActionsWidget() override;
};
