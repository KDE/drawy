/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "page.hpp"

Page::Page() = default;
Page::~Page() = default;

QString Page::name() const
{
    return mName;
}

void Page::setName(const QString &newName)
{
    mName = newName;
}
