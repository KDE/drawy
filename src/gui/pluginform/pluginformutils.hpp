/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libdrawygui_export.h"
#include <QString>
class QJsonObject;

namespace PluginFormUtils
{
LIBDRAWYGUI_EXPORT void serializePluginForm(QJsonObject &obj, const QString &pluginName, const QString &customObjectName);
LIBDRAWYGUI_EXPORT void deserializePluginForm(const QJsonObject &obj, QString &pluginName, QString &customObjectName);
[[nodiscard]] LIBDRAWYGUI_EXPORT QString pluginNameStr();
[[nodiscard]] LIBDRAWYGUI_EXPORT QString customObjectNameStr();
} // namespace PluginFormUtils
