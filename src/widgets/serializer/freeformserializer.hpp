/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "itemserializer.hpp"
#include "libdrawywidgets_private_export.h"
class FreeformItem;
class QJsonObject;
class LIBDRAWYWIDGETS_TESTS_EXPORT FreeFormSerializer : public ItemSerializer
{
public:
    explicit FreeFormSerializer(const FreeformItem *item);
    ~FreeFormSerializer() override;
    [[nodiscard]] QJsonObject serialize(int zorder) const override;
};
