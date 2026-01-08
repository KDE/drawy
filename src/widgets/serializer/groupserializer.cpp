/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "groupserializer.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
GroupSerializer::GroupSerializer(const GroupItem *item)
    : ItemSerializer(item)
{
}

GroupSerializer::~GroupSerializer() = default;

QJsonObject GroupSerializer::serialize() const
{
    QJsonObject obj = ItemSerializer::serialize();
    return obj;
}
