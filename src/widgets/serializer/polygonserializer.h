/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/polygon.hpp"
#include "itemserializer.h"
#include "libdrawywidgets_private_export.h"
class LIBDRAWYWIDGETS_TESTS_EXPORT PolygonSerializer : public ItemSerializer
{
public:
    explicit PolygonSerializer(PolygonItem *item);
    ~PolygonSerializer() override;

    [[nodiscard]] QJsonObject serialize() const override;
};
