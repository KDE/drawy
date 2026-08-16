/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "customtool.hpp"
#include "pluginform/pluginformmanager.hpp"

CustomTool::CustomTool(ApplicationContext *context)
    : PolygonDrawingTool(context)
{
}

CustomTool::~CustomTool() = default;

Tool::Type CustomTool::type() const
{
    return Tool::Type::Custom;
}

QString CustomTool::icon() const
{
    return {};
}

QList<Property::Type> CustomTool::properties() const
{
    return PolygonDrawingTool::properties();
}

void CustomTool::setUpdateTool(const PluginForm::PluginFormInfo &pluginInfo)
{
    m_itemFactory = PluginFormManager::self()->generateItemFactory(pluginInfo);
    m_properties = pluginInfo.properties;
}
