/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "basicshapesplugin.hpp"
#include "basicshapespluginutils.hpp"
#include <KLocalizedString>
#include <KPluginFactory>
K_PLUGIN_CLASS_WITH_JSON(BasicShapesPlugin, "basicshapes_plugin.json")

using namespace Qt::Literals::StringLiterals;

BasicShapesPlugin::BasicShapesPlugin(QObject *parent, const QVariantList &)
    : PluginForm{parent}
{
    // Fill list of elements
    mPluginFormInfoList = BasicShapesPluginUtils::pluginFormInfoList();
}

BasicShapesPlugin::~BasicShapesPlugin() = default;

int BasicShapesPlugin::order() const
{
    return 10;
}

QString BasicShapesPlugin::title() const
{
    return i18n("Standard Forms");
}

QString BasicShapesPlugin::pluginName() const
{
    return BasicShapesPluginUtils::pluginName();
}

std::unique_ptr<ItemFactory> BasicShapesPlugin::generateItemFactory(const PluginFormInfo &pluginInfo)
{
    return BasicShapesPluginUtils::generateItemFactory(pluginInfo.name);
}

std::shared_ptr<Item> BasicShapesPlugin::createItem(const QString &customPluginName)
{
    return BasicShapesPluginUtils::createItem(customPluginName);
}

#include "basicshapesplugin.moc"
#include "moc_basicshapesplugin.cpp"
