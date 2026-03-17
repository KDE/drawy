/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "linedeserializer.hpp"
#include "item/line.hpp"
#include "itemdeserializer.hpp"
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
LineDeserializer::LineDeserializer(LineItem *item)
    : ItemDeserializer(item)
{
}

LineDeserializer::~LineDeserializer() = default;

void LineDeserializer::deserialize(const QJsonObject &obj)
{
    ItemDeserializer::deserialize(obj);
    LineItem *polyItem = static_cast<LineItem *>(mItem);
    polyItem->setStart(toPointF(value(obj, u"start"_s)));
    polyItem->setEnd(toPointF(value(obj, u"end"_s)));
}
