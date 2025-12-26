// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ellipsefactory.hpp"

#include "../ellipse.hpp"

EllipseFactory::EllipseFactory() {
}

std::shared_ptr<Item> EllipseFactory::create() const {
    return std::make_shared<EllipseItem>();
}
