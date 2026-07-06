/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "textserializer.hpp"
#include "item/text.hpp"
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
TextSerializer::TextSerializer(const TextItem *item)
    : ItemSerializer(item)
{
}

TextSerializer::~TextSerializer() = default;

QJsonObject TextSerializer::serialize(int zorder) const
{
    QJsonObject obj = ItemSerializer::serialize(zorder);
    const TextItem *text = static_cast<const TextItem *>(mItem);
    obj[u"text"_s] = QJsonValue(text->text());
    obj[u"html"_s] = QJsonValue(text->html());
    obj[u"top_left"_s] = QJsonValue(toJson(text->normalizedBoundingBox().topLeft()));
    obj[u"wrap_width"_s] = QJsonValue(text->wrapWidth());
    return obj;
}
