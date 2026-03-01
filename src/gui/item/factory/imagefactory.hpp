/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "itemfactory.hpp"
#include "libdrawygui_export.h"

class Item;

class LIBDRAWYGUI_EXPORT ImageFactory : public ItemFactory
{
public:
    ImageFactory();
    ~ImageFactory() override = default;
    [[nodiscard]] std::shared_ptr<Item> create() const override;
};
