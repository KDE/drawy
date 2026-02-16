/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "itemserializer.hpp"
#include "libdrawywidgets_private_export.h"
class GroupItem;
class QJsonObject;
class LIBDRAWYWIDGETS_TESTS_EXPORT GroupSerializer : public ItemSerializer
{
public:
    explicit GroupSerializer(const GroupItem *item);
    ~GroupSerializer() override;
    [[nodiscard]] QJsonObject serialize(int zorder) const override;
};
