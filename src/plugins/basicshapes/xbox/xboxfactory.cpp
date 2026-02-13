/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "xboxfactory.hpp"

#include "xbox.hpp"

XBoxFactory::XBoxFactory() = default;

std::shared_ptr<Item> XBoxFactory::create() const
{
    return std::make_shared<XBoxItem>();
}
