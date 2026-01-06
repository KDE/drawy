// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "textfactory.hpp"

#include "item/text.hpp"

TextFactory::TextFactory() = default;

std::shared_ptr<Item> TextFactory::create() const
{
    return std::make_shared<TextItem>();
}
