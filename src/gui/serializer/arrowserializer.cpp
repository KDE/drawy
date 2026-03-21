/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "arrowserializer.hpp"
#include "item/arrow/arrow.hpp"
#include "item/arrow/arrowutils.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
ArrowSerializer::ArrowSerializer(const ArrowItem *item)
    : LineSerializer(item)
{
}

ArrowSerializer::~ArrowSerializer() = default;

QJsonObject ArrowSerializer::serialize(int zorder) const
{
    const ArrowItem *arrow = dynamic_cast<const ArrowItem *>(mItem);
    QJsonObject obj = LineSerializer::serialize(zorder);
    // obj[u"startArrow"_s] = ArrowUtils::convertArrowTypeEnumToString(arrow->startArrow());
    // obj[u"endArrow"_s] = ArrowUtils::convertArrowTypeEnumToString(arrow->endArrow());
    return obj;
}
