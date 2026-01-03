// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "freeformfactory.hpp"

#include "item/freeform.hpp"

FreeformFactory::FreeformFactory()
{
}
std::shared_ptr<Item> FreeformFactory::create() const
{
    return std::make_shared<FreeformItem>();
}
