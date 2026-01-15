/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "groupdeserializer.hpp"
#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
GroupDeserializer::GroupDeserializer(GroupItem *item)
    : ItemDeserializer(item)
{
}

GroupDeserializer::~GroupDeserializer() = default;

void GroupDeserializer::deserialize(const QJsonObject &obj)
{
    ItemDeserializer::deserialize(obj);
    const QJsonArray items = obj[u"items"_s].toArray();
    for (const auto &val : items) {
        const QJsonObject item = val.toObject();
        // Deserialize each item
    }
}
