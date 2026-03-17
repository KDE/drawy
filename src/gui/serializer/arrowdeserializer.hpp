/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawygui_export.h"
#include "linedeserializer.hpp"
class ArrowItem;

class LIBDRAWYGUI_EXPORT ArrowDeserializer : public LineDeserializer
{
public:
    explicit ArrowDeserializer(ArrowItem *item);
    ~ArrowDeserializer() override;

    void deserialize(const QJsonObject &obj) override;
};
