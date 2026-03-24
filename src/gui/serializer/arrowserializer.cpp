/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "arrowserializer.hpp"
#include "item/arrow/arrow.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
ArrowSerializer::ArrowSerializer(const ArrowItem *item)
    : LineSerializer(item)
{
}

ArrowSerializer::~ArrowSerializer() = default;

QJsonObject ArrowSerializer::serialize(int zorder) const
{
    QJsonObject obj = LineSerializer::serialize(zorder);
    return obj;
}
