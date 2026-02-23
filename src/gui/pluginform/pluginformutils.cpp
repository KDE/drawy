/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pluginformutils.hpp"
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
void PluginFormUtils::serializePluginForm(QJsonObject &obj, const QString &pluginName, const QString &customObjectName)
{
    obj[pluginNameStr()] = pluginName;
    obj[customObjectNameStr()] = customObjectName;
}

void PluginFormUtils::deserializePluginForm(const QJsonObject &obj, QString &pluginName, QString &customObjectName)
{
    pluginName = obj[pluginNameStr()].toString();
    customObjectName = obj[customObjectNameStr()].toString();
}

QString PluginFormUtils::pluginNameStr()
{
    return u"pluginName"_s;
}

QString PluginFormUtils::customObjectNameStr()
{
    return u"customObjectName"_s;
}
