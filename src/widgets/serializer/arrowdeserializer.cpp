/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "arrowdeserializer.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
ArrowDeserializer::ArrowDeserializer(ArrowItem *item)
    : PolygonDeserializer(item)
{
}

ArrowDeserializer::~ArrowDeserializer() = default;

void ArrowDeserializer::deserialize(const QJsonObject &obj)
{
    PolygonDeserializer::deserialize(obj);
}
