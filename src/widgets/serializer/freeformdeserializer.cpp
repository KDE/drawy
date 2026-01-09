/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "freeformdeserializer.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
FreeformDeserializer::FreeformDeserializer(FreeformItem *item)
    : ItemDeserializer(item)
{
}

FreeformDeserializer::~FreeformDeserializer() = default;

void FreeformDeserializer::deserialize(const QJsonObject &obj)
{
}
