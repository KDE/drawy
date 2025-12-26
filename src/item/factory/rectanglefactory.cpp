// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "rectanglefactory.hpp"

#include "../rectangle.hpp"

RectangleFactory::RectangleFactory() {
}

std::shared_ptr<Item> RectangleFactory::create() const {
    return std::make_shared<RectangleItem>();
}
