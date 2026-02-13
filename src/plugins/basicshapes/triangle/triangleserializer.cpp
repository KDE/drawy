/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "triangleserializer.hpp"
#include "basicshapespluginutils.hpp"
#include "pluginform/pluginformutils.hpp"
#include "triangle.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
TriangleSerializer::TriangleSerializer(const TriangleItem *item)
    : PolygonSerializer(item)
{
}

TriangleSerializer::~TriangleSerializer() = default;

QJsonObject TriangleSerializer::serialize(int zorder) const
{
    QJsonObject obj = PolygonSerializer::serialize(zorder);
    PluginFormUtils::serializePluginForm(
        obj,
        BasicShapesPluginUtils::pluginName(),
        BasicShapesPluginUtils::convertStandardFormPluginTypeToString(BasicShapesPluginUtils::BasicShapesPluginType::Triangle));
    return obj;
}
