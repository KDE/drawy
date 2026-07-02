/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "ellipsedeserializer.hpp"
#include "item/ellipse.hpp"
using namespace Qt::Literals::StringLiterals;
EllipseDeserializer::EllipseDeserializer(EllipseItem *item)
    : PolygonDeserializer(item)
{
}

EllipseDeserializer::~EllipseDeserializer() = default;
