/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "itemdeserializer.hpp"
#include "libdrawywidgets_export.h"
class TextItem;
class QJsonObject;
class LIBDRAWYWIDGETS_EXPORT TextDeserializer : public ItemDeserializer
{
public:
    explicit TextDeserializer(TextItem *item);
    ~TextDeserializer() override;

    void deserialize(const QJsonObject &obj) override;
};
