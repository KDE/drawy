/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawygui_export.h"
#include "polygondeserializer.hpp"
class DiamondItem;
class QJsonObject;
class LIBDRAWYGUI_EXPORT DiamondDeserializer : public PolygonDeserializer
{
public:
    explicit DiamondDeserializer(DiamondItem *item);
    ~DiamondDeserializer() override;
};
