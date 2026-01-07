/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "freeformserializer.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
FreeFormSerializer::FreeFormSerializer(FreeformItem *item)
    : ItemSerializer(item)
{
}

FreeFormSerializer::~FreeFormSerializer() = default;

QJsonObject FreeFormSerializer::serialize() const
{
    QJsonObject obj = ItemSerializer::serialize();
    FreeformItem *freeform = static_cast<FreeformItem *>(mItem);
    obj[u"points"_s] = toJson(freeform->points());
    obj[u"pressures"_s] = toJson(freeform->pressures());
    return obj;
}
