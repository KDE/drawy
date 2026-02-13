/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libdrawywidgets_export.h"
#include <QString>
class QJsonObject;

namespace PluginFormUtils
{
LIBDRAWYWIDGETS_EXPORT void serializePluginForm(QJsonObject &obj, const QString &pluginName, const QString &customObjectName);
LIBDRAWYWIDGETS_EXPORT void deserializePluginForm(const QJsonObject &obj, QString &pluginName, QString &customObjectName);
[[nodiscard]] LIBDRAWYWIDGETS_EXPORT QString pluginNameStr();
[[nodiscard]] LIBDRAWYWIDGETS_EXPORT QString customObjectNameStr();
};
