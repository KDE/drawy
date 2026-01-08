/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "ellipseserializer.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
EllipseSerializer::EllipseSerializer(const EllipseItem *item)
    : PolygonSerializer(item)
{
}

EllipseSerializer::~EllipseSerializer() = default;
