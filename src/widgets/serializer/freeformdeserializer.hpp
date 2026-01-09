/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/freeform.hpp"
#include "itemdeserializer.hpp"
#include "libdrawywidgets_private_export.h"
class LIBDRAWYWIDGETS_TESTS_EXPORT FreeformDeserializer : public ItemDeserializer
{
public:
    explicit FreeformDeserializer(FreeformItem *item);
    ~FreeformDeserializer() override;

    void deserialize(const QJsonObject &obj) override;
};
