/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/text.hpp"
#include "itemserializer.hpp"
#include "libdrawywidgets_private_export.h"
class LIBDRAWYWIDGETS_TESTS_EXPORT TextSerializer : public ItemSerializer
{
public:
    explicit TextSerializer(const TextItem *item);
    ~TextSerializer() override;
    [[nodiscard]] QJsonObject serialize() const override;
};
