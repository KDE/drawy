/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawygui_export.h"
#include "polygondeserializer.hpp"
class RectangleItem;
class QJsonObject;
class LIBDRAWYGUI_EXPORT RectangleDeserializer : public PolygonDeserializer
{
public:
    explicit RectangleDeserializer(RectangleItem *item);
    ~RectangleDeserializer() override;

    void deserialize(const QJsonObject &obj) override;
};
