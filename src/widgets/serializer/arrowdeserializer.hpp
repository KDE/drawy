/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/arrow.hpp"
#include "libdrawywidgets_private_export.h"
#include "polygondeserializer.hpp"
class LIBDRAWYWIDGETS_TESTS_EXPORT ArrowDeserializer : public PolygonDeserializer
{
public:
    explicit ArrowDeserializer(ArrowItem *item);
    ~ArrowDeserializer() override;

    void deserialize(const QJsonObject &obj) override;
};
