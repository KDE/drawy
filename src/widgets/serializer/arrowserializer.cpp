/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "arrowserializer.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
ArrowSerializer::ArrowSerializer(const std::shared_ptr<ArrowItem> &item)
    : PolygonSerializer(item)
{
}

ArrowSerializer::~ArrowSerializer() = default;
