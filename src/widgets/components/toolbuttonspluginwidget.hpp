/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "libdrawywidgets_private_export.h"
#include "pluginform/pluginform.hpp"
#include <QObject>
class Tool;
class ToolButtonSelectPlugin;
class ToolButtonPlugin;
class CustomTool;
class LIBDRAWYWIDGETS_TESTS_EXPORT ToolButtonsPluginWidget : public QObject
{
    Q_OBJECT
public:
    explicit ToolButtonsPluginWidget(QWidget *parentWidget, QObject *parent = nullptr);
    ~ToolButtonsPluginWidget() override;

    [[nodiscard]] ToolButtonSelectPlugin *toolButtonSelectPlugin() const;

    [[nodiscard]] ToolButtonPlugin *toolButtonPlugin() const;

    void setTool(const std::shared_ptr<CustomTool> &newTool);

Q_SIGNALS:
    void toolChanged(Tool &);

private:
    LIBDRAWYWIDGETS_NO_EXPORT void slotToolActivated(const PluginForm::PluginFormInfo &item);
    ToolButtonSelectPlugin *const mToolButtonSelectPlugin;
    ToolButtonPlugin *const mToolButtonPlugin;
    std::shared_ptr<CustomTool> mTool;
};
