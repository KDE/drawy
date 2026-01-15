/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "diamonddeserializer.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
DiamondDeserializer::DiamondDeserializer(DiamondItem *item)
    : PolygonDeserializer(item)
{
}

DiamondDeserializer::~DiamondDeserializer() = default;

void DiamondDeserializer::deserialize(const QJsonObject &obj)
{
    PolygonDeserializer::deserialize(obj);
}
