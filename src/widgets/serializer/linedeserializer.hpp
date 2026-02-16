/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_private_export.h"
#include "polygondeserializer.hpp"
class LineItem;
class QJsonObject;
class LIBDRAWYWIDGETS_TESTS_EXPORT LineDeserializer : public PolygonDeserializer
{
public:
    explicit LineDeserializer(LineItem *item);
    ~LineDeserializer() override;

    void deserialize(const QJsonObject &obj) override;
};
