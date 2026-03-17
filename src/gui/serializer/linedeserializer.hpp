/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawygui_export.h"
#include "polygondeserializer.hpp"
class LineItem;
class LIBDRAWYGUI_EXPORT LineDeserializer : public ItemDeserializer
{
public:
    explicit LineDeserializer(LineItem *item);
    ~LineDeserializer() override;

    void deserialize(const QJsonObject &obj) override;
};
