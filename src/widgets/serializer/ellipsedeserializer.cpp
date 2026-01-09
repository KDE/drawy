/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "ellipsedeserializer.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
EllipseDeserializer::EllipseDeserializer(EllipseItem *item)
    : PolygonDeserializer(item)
{
}

EllipseDeserializer::~EllipseDeserializer() = default;

void EllipseDeserializer::deserialize(const QJsonObject &obj)
{
    PolygonDeserializer::deserialize(obj);
}
