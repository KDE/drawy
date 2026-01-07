/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/rectangle.hpp"
#include "libdrawywidgets_private_export.h"
#include "polygonserializer.h"
class LIBDRAWYWIDGETS_TESTS_EXPORT RectangleSerializer : public PolygonSerializer
{
public:
    explicit RectangleSerializer(const std::shared_ptr<RectangleItem> &item);
    ~RectangleSerializer() override;
};
