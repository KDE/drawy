/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "serializer/polygondeserializer.hpp"
class QJsonObject;
class TriangleItem;
class TriangleDeserializer : public PolygonDeserializer
{
public:
    explicit TriangleDeserializer(TriangleItem *item);
    ~TriangleDeserializer() override;
};
