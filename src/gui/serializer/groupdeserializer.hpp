/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "itemdeserializer.hpp"
#include "libdrawygui_export.h"
class GroupItem;
class QJsonObject;
class LIBDRAWYGUI_EXPORT GroupDeserializer : public ItemDeserializer
{
public:
    explicit GroupDeserializer(GroupItem *item);
    ~GroupDeserializer() override;

    void deserialize(const QJsonObject &obj) override;
};
