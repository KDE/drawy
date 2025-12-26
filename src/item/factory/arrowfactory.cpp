// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "arrowfactory.hpp"

#include "../arrow.hpp"

ArrowFactory::ArrowFactory() {
}

std::shared_ptr<Item> ArrowFactory::create() const {
    return std::make_shared<ArrowItem>();
}
