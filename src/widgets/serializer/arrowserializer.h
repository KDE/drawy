/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/arrow.hpp"
#include "libdrawywidgets_private_export.h"
#include "polygonserializer.h"
class LIBDRAWYWIDGETS_TESTS_EXPORT ArrowSerializer : public PolygonSerializer
{
public:
    explicit ArrowSerializer(const std::shared_ptr<ArrowItem> &item);
    ~ArrowSerializer() override;
};
