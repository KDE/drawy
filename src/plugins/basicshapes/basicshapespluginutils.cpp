/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "basicshapespluginutils.hpp"
#include "basicshapesformplugin_debug.h"
#include "triangle/triangle.hpp"
#include "triangle/trianglefactory.hpp"
#include "xbox/xbox.hpp"
#include "xbox/xboxfactory.hpp"

using namespace Qt::Literals::StringLiterals;

QString BasicShapesPluginUtils::pluginName()
{
    return u"standardforms"_s;
}

QString BasicShapesPluginUtils::convertStandardFormPluginTypeToString(BasicShapesPluginUtils::BasicShapesPluginType type)
{
    switch (type) {
    case BasicShapesPluginUtils::BasicShapesPluginType::Triangle:
        return u"TRIANGLE"_s;
    case BasicShapesPluginUtils::BasicShapesPluginType::XBox:
        return u"XBOX"_s;
    case BasicShapesPluginUtils::BasicShapesPluginType::Unknown:
        qCWarning(BASICSHAPES_FORM_LOG) << "Incorrect StandardFormPluginType Unknown it's a bug";
        break;
    }
    Q_UNREACHABLE();
    return {};
}

BasicShapesPluginUtils::BasicShapesPluginType BasicShapesPluginUtils::convertStandardFormPluginTypeFromString(const QString &str)
{
    if (str == u"TRIANGLE"_s) {
        return BasicShapesPluginUtils::BasicShapesPluginType::Triangle;
    } else if (str == u"XBOX"_s) {
        return BasicShapesPluginUtils::BasicShapesPluginType::XBox;
    } else {
        qCWarning(BASICSHAPES_FORM_LOG) << "Incorrect StandardFormPluginType name " << str;
    }
    return BasicShapesPluginUtils::BasicShapesPluginType::Unknown;
}

std::unique_ptr<ItemFactory> BasicShapesPluginUtils::generateItemFactory(const QString &name)
{
    switch (BasicShapesPluginUtils::convertStandardFormPluginTypeFromString(name)) {
    case BasicShapesPluginUtils::BasicShapesPluginType::Triangle:
        return std::make_unique<TriangleFactory>();
    case BasicShapesPluginUtils::BasicShapesPluginType::XBox:
        return std::make_unique<XBoxFactory>();
    case BasicShapesPluginUtils::BasicShapesPluginType::Unknown:
        return {};
    }
    Q_UNREACHABLE();
    return {};
}

QList<PluginForm::PluginFormInfo> BasicShapesPluginUtils::pluginFormInfoList()
{
    return {TriangleItem::pluginFormInfo(), XBoxItem::pluginFormInfo()};
}

std::shared_ptr<Item> BasicShapesPluginUtils::createItem(const QString &customPluginName)
{
    const BasicShapesPluginUtils::BasicShapesPluginType type = convertStandardFormPluginTypeFromString(customPluginName);
    switch (type) {
    case BasicShapesPluginUtils::BasicShapesPluginType::Triangle:
        return std::make_shared<TriangleItem>();
    case BasicShapesPluginUtils::BasicShapesPluginType::XBox: {
        return std::make_shared<XBoxItem>();
    }
    case BasicShapesPluginUtils::BasicShapesPluginType::Unknown:
        return {};
    }
    return {};
}
