/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "diamondfactory.hpp"

#include "item/diamond.hpp"

DiamondFactory::DiamondFactory() = default;

std::shared_ptr<Item> DiamondFactory::create() const
{
    return std::make_shared<DiamondItem>();
}
