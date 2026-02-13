/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "item/factory/itemfactory.hpp"
#include "libstandardformplugin_export.h"
#include "pluginform/pluginform.hpp"
#include <QString>
namespace BasicShapesPluginUtils
{
enum class BasicShapesPluginType : int8_t {
    Unknown = 0,
    Triangle,
    XBox,
};

[[nodiscard]] LIBSTANDARDFORMPLUGIN_EXPORT QString pluginName();

[[nodiscard]] LIBSTANDARDFORMPLUGIN_EXPORT QString convertStandardFormPluginTypeToString(BasicShapesPluginUtils::BasicShapesPluginType type);

[[nodiscard]] LIBSTANDARDFORMPLUGIN_EXPORT BasicShapesPluginUtils::BasicShapesPluginType convertStandardFormPluginTypeFromString(const QString &str);

[[nodiscard]] LIBSTANDARDFORMPLUGIN_EXPORT std::unique_ptr<ItemFactory> generateItemFactory(const QString &name);
[[nodiscard]] LIBSTANDARDFORMPLUGIN_EXPORT QList<PluginForm::PluginFormInfo> pluginFormInfoList();
[[nodiscard]] LIBSTANDARDFORMPLUGIN_EXPORT std::shared_ptr<Item> createItem(const QString &customPluginName);
};
