/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "libdrawywidgets_private_export.h"
#include "pluginform/pluginform.hpp"
#include "tools/polygondrawingtool.hpp"
class LIBDRAWYWIDGETS_TESTS_EXPORT CustomTool : public PolygonDrawingTool
{
public:
    explicit CustomTool(ApplicationContext *context);
    ~CustomTool() override;
    [[nodiscard]] Tool::Type type() const override;
    [[nodiscard]] QString icon() const override;
    [[nodiscard]] QList<Property::Type> properties() const override;

    void setUpdateTool(const PluginForm::PluginFormInfo &pluginInfo);
};
