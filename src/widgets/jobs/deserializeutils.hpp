/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "item/item.hpp"
#include <QList>
#include <memory>
class QJsonArray;

namespace DeserializeUtils
{
[[nodiscard]] QList<std::shared_ptr<Item>> deserializeItems(const QJsonArray &items);
};
