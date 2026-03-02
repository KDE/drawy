/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "item/factory/itemfactory.hpp"
#include "libbasicshapesformplugin_export.h"
#include "pluginform/pluginform.hpp"
#include <QString>
namespace BasicShapesPluginUtils
{
enum class BasicShapesPluginType : int8_t {
    Unknown = 0,
    Triangle,
    XBox,
};

[[nodiscard]] LIBBASICSHAPESFORMPLUGIN_EXPORT QString pluginName();

[[nodiscard]] LIBBASICSHAPESFORMPLUGIN_EXPORT QString convertStandardFormPluginTypeToString(BasicShapesPluginUtils::BasicShapesPluginType type);

[[nodiscard]] LIBBASICSHAPESFORMPLUGIN_EXPORT BasicShapesPluginUtils::BasicShapesPluginType convertStandardFormPluginTypeFromString(const QString &str);

[[nodiscard]] LIBBASICSHAPESFORMPLUGIN_EXPORT std::unique_ptr<ItemFactory> generateItemFactory(const QString &name);
[[nodiscard]] LIBBASICSHAPESFORMPLUGIN_EXPORT QList<PluginForm::PluginFormInfo> pluginFormInfoList();
[[nodiscard]] LIBBASICSHAPESFORMPLUGIN_EXPORT std::shared_ptr<Item> createItem(const QString &customPluginName);
};
