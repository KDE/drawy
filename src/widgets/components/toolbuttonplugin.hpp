/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QToolButton>

class ToolButtonPlugin : public QToolButton
{
    Q_OBJECT
public:
    explicit ToolButtonPlugin(QWidget *parent = nullptr);
    ~ToolButtonPlugin() override;
};
