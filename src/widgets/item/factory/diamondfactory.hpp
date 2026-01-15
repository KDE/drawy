/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include "itemfactory.hpp"

class Item;

class DiamondFactory : public ItemFactory
{
public:
    DiamondFactory();
    ~DiamondFactory() override = default;
    std::shared_ptr<Item> create() const override;
};
