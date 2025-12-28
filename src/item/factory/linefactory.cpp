// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "linefactory.hpp"

#include "../line.hpp"

LineFactory::LineFactory()
{
}

std::shared_ptr<Item> LineFactory::create() const
{
    return std::make_shared<LineItem>();
}
