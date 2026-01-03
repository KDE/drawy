// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <memory>

class Item;

class ItemFactory
{
public:
    ItemFactory();
    virtual ~ItemFactory() = default;
    virtual std::shared_ptr<Item> create() const = 0;
};
