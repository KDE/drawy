/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "libdrawygui_export.h"
#include <QByteArray>
#include <QString>
class QJsonObject;
class QPointF;
namespace SerializerUtils
{
[[nodiscard]] LIBDRAWYGUI_EXPORT int version();
[[nodiscard]] LIBDRAWYGUI_EXPORT int pageVersion();
[[nodiscard]] LIBDRAWYGUI_EXPORT QJsonObject toJson(const QPointF &point);
[[nodiscard]] LIBDRAWYGUI_EXPORT QByteArray compressData(const QJsonObject &obj);
LIBDRAWYGUI_EXPORT void saveInFile(const QJsonObject &obj, const QString &filename);
} // namespace SerializerUtils
