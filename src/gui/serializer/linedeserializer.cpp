/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "linedeserializer.hpp"
#include "item/line.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
LineDeserializer::LineDeserializer(LineItem *item)
    : PolygonDeserializer(item)
{
}

LineDeserializer::~LineDeserializer() = default;
