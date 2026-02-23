// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ellipsefactory.hpp"

#include "item/ellipse.hpp"

EllipseFactory::EllipseFactory() = default;

std::shared_ptr<Item> EllipseFactory::create() const
{
    return std::make_shared<EllipseItem>();
}
