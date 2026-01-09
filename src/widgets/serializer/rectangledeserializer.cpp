/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "rectangledeserializer.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
RectangleDeserializer::RectangleDeserializer(RectangleItem *item)
    : PolygonDeserializer(item)
{
}

RectangleDeserializer::~RectangleDeserializer() = default;

void RectangleDeserializer::deserialize(const QJsonObject &obj)
{
    PolygonDeserializer::deserialize(obj);
}
