/*
 * SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/image.hpp"
#include "itemserializer.hpp"
#include "libdrawygui_export.h"

class LIBDRAWYGUI_EXPORT ImageSerializer : public ItemSerializer
{
public:
    explicit ImageSerializer(const ImageItem *item);
    ~ImageSerializer() override;
    [[nodiscard]] QJsonObject serialize(int zorder) const override;
};
