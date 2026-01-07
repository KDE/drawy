/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "ellipseserializer.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
EllipseSerializer::EllipseSerializer(const std::shared_ptr<EllipseItem> &item)
    : PolygonSerializer(item)
{
}

EllipseSerializer::~EllipseSerializer() = default;
