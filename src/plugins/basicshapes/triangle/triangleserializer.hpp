/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "serializer/polygonserializer.hpp"
class TriangleItem;
class TriangleSerializer : public PolygonSerializer
{
public:
    explicit TriangleSerializer(const TriangleItem *item);
    ~TriangleSerializer() override;
    [[nodiscard]] QJsonObject serialize(int zorder) const override;
};
