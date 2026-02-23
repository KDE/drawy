/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawygui_export.h"
#include "polygondeserializer.hpp"
class EllipseItem;
class QJsonObject;
class LIBDRAWYGUI_EXPORT EllipseDeserializer : public PolygonDeserializer
{
public:
    explicit EllipseDeserializer(EllipseItem *item);
    ~EllipseDeserializer() override;
};
