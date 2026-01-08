/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "lineserializer.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
LineSerializer::LineSerializer(const LineItem *item)
    : PolygonSerializer(item)
{
}

LineSerializer::~LineSerializer() = default;
