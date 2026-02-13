/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "serializer/polygonserializer.hpp"
class XBoxItem;
class QJsonObject;
class XBoxSerializer : public PolygonSerializer
{
public:
    explicit XBoxSerializer(const XBoxItem *item);
    ~XBoxSerializer() override;
    [[nodiscard]] QJsonObject serialize(int zorder) const override;
};
