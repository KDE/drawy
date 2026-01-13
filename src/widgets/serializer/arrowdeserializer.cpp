/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "arrowdeserializer.hpp"
#include "item/arrowutils.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
ArrowDeserializer::ArrowDeserializer(ArrowItem *item)
    : PolygonDeserializer(item)
{
}

ArrowDeserializer::~ArrowDeserializer() = default;

void ArrowDeserializer::deserialize(const QJsonObject &obj)
{
    PolygonDeserializer::deserialize(obj);
    ArrowItem *arrowItem = static_cast<ArrowItem *>(mItem);
    if (!obj.contains(u"startArrow")) {
        arrowItem->setStartArrow(ArrowItem::ArrowType::None);
    } else {
        arrowItem->setStartArrow(ArrowUtils::convertArrowTypeStringToArrowEnum(obj[u"startArrow"].toString()));
    }
    if (!obj.contains(u"endArrow")) {
        arrowItem->setEndArrow(ArrowItem::ArrowType::Arrow);
    } else {
        arrowItem->setEndArrow(ArrowUtils::convertArrowTypeStringToArrowEnum(obj[u"endArrow"].toString()));
    }
}
