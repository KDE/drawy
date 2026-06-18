/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "triangledeserializer.hpp"
#include "triangle.hpp"
using namespace Qt::Literals::StringLiterals;
TriangleDeserializer::TriangleDeserializer(TriangleItem *item)
    : PolygonDeserializer(item)
{
}

TriangleDeserializer::~TriangleDeserializer() = default;
