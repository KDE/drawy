/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "textserializer.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
TextSerializer::TextSerializer(const TextItem *item)
    : ItemSerializer(item)
{
}

TextSerializer::~TextSerializer() = default;

QJsonObject TextSerializer::serialize() const
{
    QJsonObject obj = ItemSerializer::serialize();
    const TextItem *text = static_cast<const TextItem *>(mItem);
    obj[u"text"_s] = QJsonValue(text->text());
    return obj;
}
