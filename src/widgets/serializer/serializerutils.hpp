/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
class QJsonObject;
class QPointF;
namespace SerializerUtils
{
[[nodiscard]] int version();
[[nodiscard]] QJsonObject toJson(const QPointF &point);
};
