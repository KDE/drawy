/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawygui_export.h"
#include "polygonserializer.hpp"
class DiamondItem;
class LIBDRAWYGUI_EXPORT DiamondSerializer : public PolygonSerializer
{
public:
    explicit DiamondSerializer(const DiamondItem *item);
    ~DiamondSerializer() override;
};
