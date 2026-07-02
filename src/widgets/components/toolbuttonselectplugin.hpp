/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "pluginform/pluginform.hpp"
#include <QToolButton>

class ToolButtonSelectPlugin : public QToolButton
{
    Q_OBJECT
public:
    explicit ToolButtonSelectPlugin(QWidget *parent = nullptr);
    ~ToolButtonSelectPlugin() override;

Q_SIGNALS:
    void toolActivated(const PluginForm::PluginFormInfo &item);

private:
    void fillMenu();
};
