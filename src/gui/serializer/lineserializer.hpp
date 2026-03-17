/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawygui_export.h"
#include "polygonserializer.hpp"
class LineItem;
class LIBDRAWYGUI_EXPORT LineSerializer : public ItemSerializer
{
public:
    explicit LineSerializer(const LineItem *item);
    ~LineSerializer() override;

    [[nodiscard]] QJsonObject serialize(int zorder) const override;
};
