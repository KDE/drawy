/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawygui_export.h"
#include "polygonserializer.hpp"
class ArrowItem;
class QJsonObject;
class LIBDRAWYGUI_EXPORT ArrowSerializer : public PolygonSerializer
{
public:
    explicit ArrowSerializer(const ArrowItem *item);
    ~ArrowSerializer() override;
    [[nodiscard]] QJsonObject serialize(int zorder) const override;
};
