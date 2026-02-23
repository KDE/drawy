// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "itemfactory.hpp"
#include "libdrawygui_export.h"

class LIBDRAWYGUI_EXPORT FreeformFactory : public ItemFactory
{
public:
    FreeformFactory();
    ~FreeformFactory() override = default;
    [[nodiscard]] std::shared_ptr<Item> create() const override;
};
