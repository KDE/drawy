/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "lineserializer.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
LineSerializer::LineSerializer(const std::shared_ptr<LineItem> &item)
    : PolygonSerializer(item)
{
}

LineSerializer::~LineSerializer() = default;
