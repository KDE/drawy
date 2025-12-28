// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "itemfactory.hpp"

class ArrowFactory : public ItemFactory
{
public:
    ArrowFactory();
    ~ArrowFactory() override = default;
    std::shared_ptr<Item> create() const override;
};
