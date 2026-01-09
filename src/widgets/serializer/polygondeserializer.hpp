/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/polygon.hpp"
#include "itemdeserializer.hpp"
#include "libdrawywidgets_private_export.h"
class LIBDRAWYWIDGETS_TESTS_EXPORT PolygonDeserializer : public ItemDeserializer
{
public:
    explicit PolygonDeserializer(PolygonItem *item);
    ~PolygonDeserializer() override;

    void deserialize(const QJsonObject &obj) override;
};
