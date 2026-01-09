/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/ellipse.hpp"
#include "libdrawywidgets_private_export.h"
#include "polygondeserializer.hpp"
class LIBDRAWYWIDGETS_TESTS_EXPORT EllipseDeserializer : public PolygonDeserializer
{
public:
    explicit EllipseDeserializer(EllipseItem *item);
    ~EllipseDeserializer() override;

    void deserialize(const QJsonObject &obj) override;
};
