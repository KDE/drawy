/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "rectangleserializer.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
RectangleSerializer::RectangleSerializer(RectangleItem *item)
    : PolygonSerializer(item)
{
}

RectangleSerializer::~RectangleSerializer() = default;
