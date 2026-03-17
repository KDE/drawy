/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "arrowdeserializer.hpp"
#include "item/arrow.hpp"
#include "item/arrowutils.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
ArrowDeserializer::ArrowDeserializer(ArrowItem *item)
    : LineDeserializer(item)
{
}

ArrowDeserializer::~ArrowDeserializer() = default;

void ArrowDeserializer::deserialize(const QJsonObject &obj)
{
    LineDeserializer::deserialize(obj);
    ArrowItem *arrowItem = static_cast<ArrowItem *>(mItem);
    if (!obj.contains(u"startArrow")) {
        arrowItem->setStartArrow(ArrowUtils::ArrowType::None);
    } else {
        arrowItem->setStartArrow(ArrowUtils::convertArrowTypeStringToArrowEnum(obj[u"startArrow"].toString()));
    }
    if (!obj.contains(u"endArrow")) {
        arrowItem->setEndArrow(ArrowUtils::ArrowType::Arrow);
    } else {
        arrowItem->setEndArrow(ArrowUtils::convertArrowTypeStringToArrowEnum(obj[u"endArrow"].toString()));
    }
}
