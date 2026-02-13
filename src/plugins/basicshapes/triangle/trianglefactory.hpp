/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "item/factory/itemfactory.hpp"
#include "libstandardformplugin_export.h"
class Item;

class LIBSTANDARDFORMPLUGIN_EXPORT TriangleFactory : public ItemFactory
{
public:
    TriangleFactory();
    ~TriangleFactory() override = default;
    [[nodiscard]] std::shared_ptr<Item> create() const override;
};
