/*
 * SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/image.hpp"
#include "itemdeserializer.hpp"
#include "libdrawygui_export.h"

class LIBDRAWYGUI_EXPORT ImageDeserializer : public ItemDeserializer
{
public:
    explicit ImageDeserializer(ImageItem *item);
    ~ImageDeserializer() override;

    void deserialize(const QJsonObject &obj) override;
};
