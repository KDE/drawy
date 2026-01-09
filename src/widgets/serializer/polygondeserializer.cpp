/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "polygondeserializer.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
PolygonDeserializer::PolygonDeserializer(PolygonItem *item)
    : ItemDeserializer(item)
{
}

PolygonDeserializer::~PolygonDeserializer() = default;

void PolygonDeserializer::deserialize(const QJsonObject &obj)
{
    ItemDeserializer::deserialize(obj);
    PolygonItem *polyItem = static_cast<PolygonItem *>(mItem);
    polyItem->setStart(toPointF(value(obj, u"start"_s)));
    polyItem->setEnd(toPointF(value(obj, u"end"_s)));
}
