/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/ellipse.hpp"
#include "libdrawywidgets_private_export.h"
#include "polygonserializer.hpp"
class LIBDRAWYWIDGETS_TESTS_EXPORT EllipseSerializer : public PolygonSerializer
{
public:
    explicit EllipseSerializer(const EllipseItem *item);
    ~EllipseSerializer() override;
};
