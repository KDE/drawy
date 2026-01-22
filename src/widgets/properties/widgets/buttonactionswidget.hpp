/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_private_export.h"
#include <QToolButton>

class LIBDRAWYWIDGETS_TESTS_EXPORT ButtonActionsWidget : public QToolButton
{
    Q_OBJECT
public:
    explicit ButtonActionsWidget(QWidget *parent = nullptr);
    ~ButtonActionsWidget() override;
};
