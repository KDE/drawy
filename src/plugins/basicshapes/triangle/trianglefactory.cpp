/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "trianglefactory.hpp"

#include "triangle.hpp"

TriangleFactory::TriangleFactory() = default;

std::shared_ptr<Item> TriangleFactory::create() const
{
    return std::make_shared<TriangleItem>();
}
