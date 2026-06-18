/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "xboxdeserializer.hpp"
#include "xbox.hpp"
using namespace Qt::Literals::StringLiterals;
XBoxDeserializer::XBoxDeserializer(XBoxItem *item)
    : PolygonDeserializer(item)
{
}

XBoxDeserializer::~XBoxDeserializer() = default;
