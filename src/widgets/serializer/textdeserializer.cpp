/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "textdeserializer.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
TextDeserializer::TextDeserializer(TextItem *item)
    : ItemDeserializer(item)
{
}

TextDeserializer::~TextDeserializer() = default;

void TextDeserializer::deserialize(const QJsonObject &obj)
{
}
