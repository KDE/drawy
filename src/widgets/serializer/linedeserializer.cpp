/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "linedeserializer.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
LineDeserializer::LineDeserializer(LineItem *item)
    : PolygonDeserializer(item)
{
}

LineDeserializer::~LineDeserializer() = default;

void LineDeserializer::deserialize(const QJsonObject &obj)
{
    PolygonDeserializer::deserialize(obj);
}
