/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "itemdeserializer.hpp"
#include "libdrawygui_export.h"
class PolygonItem;
class QJsonObject;
class LIBDRAWYGUI_EXPORT PolygonDeserializer : public ItemDeserializer
{
public:
    explicit PolygonDeserializer(PolygonItem *item);
    ~PolygonDeserializer() override;

    void deserialize(const QJsonObject &obj) override;
};
