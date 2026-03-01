/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "imagefactory.hpp"

#include "item/image.hpp"

ImageFactory::ImageFactory() = default;

std::shared_ptr<Item> ImageFactory::create() const
{
    return std::make_shared<ImageItem>();
}
