/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "itemserializer.hpp"
#include "libdrawygui_export.h"
class FreeformItem;
class QJsonObject;
class LIBDRAWYGUI_EXPORT FreeFormSerializer : public ItemSerializer
{
public:
    explicit FreeFormSerializer(const FreeformItem *item);
    ~FreeFormSerializer() override;
    [[nodiscard]] QJsonObject serialize(int zorder) const override;
};
