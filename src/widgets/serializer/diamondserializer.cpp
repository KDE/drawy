/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "diamondserializer.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
DiamondSerializer::DiamondSerializer(const DiamondItem *item)
    : PolygonSerializer(item)
{
}

DiamondSerializer::~DiamondSerializer() = default;
