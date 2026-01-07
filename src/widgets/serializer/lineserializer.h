/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/line.hpp"
#include "libdrawywidgets_private_export.h"
#include "polygonserializer.h"
class LIBDRAWYWIDGETS_TESTS_EXPORT LineSerializer : public PolygonSerializer
{
public:
    explicit LineSerializer(LineItem *item);
    ~LineSerializer() override;
};
