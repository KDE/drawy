/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_export.h"
#include "polygonserializer.hpp"
class DiamondItem;
class LIBDRAWYWIDGETS_EXPORT DiamondSerializer : public PolygonSerializer
{
public:
    explicit DiamondSerializer(const DiamondItem *item);
    ~DiamondSerializer() override;
};
