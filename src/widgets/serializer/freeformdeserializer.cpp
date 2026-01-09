/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "freeformdeserializer.hpp"
#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
FreeformDeserializer::FreeformDeserializer(FreeformItem *item)
    : ItemDeserializer(item)
{
}

FreeformDeserializer::~FreeformDeserializer() = default;

void FreeformDeserializer::deserialize(const QJsonObject &obj)
{
    ItemDeserializer::deserialize(obj);
    QJsonArray points = array(value(obj, u"points"_s));
    QJsonArray pressures = array(value(obj, u"pressures"_s));

    qsizetype len{points.size()};
    FreeformItem *freeFormItem = static_cast<FreeformItem *>(mItem);
    for (qsizetype pos{0}; pos < len; pos++) {
        QPointF point{toPointF(points[pos])};

        freeFormItem->addPoint(point, pressures[pos].toDouble());
    }
}
