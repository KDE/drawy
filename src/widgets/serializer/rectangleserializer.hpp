/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_export.h"
#include "polygonserializer.hpp"
class RectangleItem;
class LIBDRAWYWIDGETS_EXPORT RectangleSerializer : public PolygonSerializer
{
public:
    explicit RectangleSerializer(const RectangleItem *item);
    ~RectangleSerializer() override;
};
