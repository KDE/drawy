/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "itemserializer.hpp"
#include "libdrawywidgets_export.h"
class TextItem;
class QJsonObject;
class LIBDRAWYWIDGETS_EXPORT TextSerializer : public ItemSerializer
{
public:
    explicit TextSerializer(const TextItem *item);
    ~TextSerializer() override;
    [[nodiscard]] QJsonObject serialize(int zorder) const override;
};
