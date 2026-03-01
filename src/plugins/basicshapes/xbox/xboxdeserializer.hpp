/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "serializer/polygondeserializer.hpp"
class XBoxItem;
class XBoxDeserializer : public PolygonDeserializer
{
public:
    explicit XBoxDeserializer(XBoxItem *item);
    ~XBoxDeserializer() override;
};
