/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "xboxserializer.hpp"
#include "basicshapespluginutils.hpp"
#include "pluginform/pluginformutils.hpp"
#include "xbox.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
XBoxSerializer::XBoxSerializer(const XBoxItem *item)
    : PolygonSerializer(item)
{
}

XBoxSerializer::~XBoxSerializer() = default;

QJsonObject XBoxSerializer::serialize(int zorder) const
{
    QJsonObject obj = PolygonSerializer::serialize(zorder);
    PluginFormUtils::serializePluginForm(obj,
                                         BasicShapesPluginUtils::pluginName(),
                                         BasicShapesPluginUtils::convertStandardFormPluginTypeToString(BasicShapesPluginUtils::BasicShapesPluginType::XBox));
    return obj;
}
