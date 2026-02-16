/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "arrowserializer.hpp"
#include "item/arrow.hpp"
#include "item/arrowutils.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
ArrowSerializer::ArrowSerializer(const ArrowItem *item)
    : PolygonSerializer(item)
{
}

ArrowSerializer::~ArrowSerializer() = default;

QJsonObject ArrowSerializer::serialize(int zorder) const
{
    const ArrowItem *arrow = dynamic_cast<const ArrowItem *>(mItem);
    QJsonObject obj = PolygonSerializer::serialize(zorder);
    obj[u"startArrow"_s] = ArrowUtils::convertArrowTypeEnumToString(arrow->startArrow());
    obj[u"endArrow"_s] = ArrowUtils::convertArrowTypeEnumToString(arrow->endArrow());
    return obj;
}
