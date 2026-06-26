/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "textdeserializer.hpp"
#include "item/text.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
TextDeserializer::TextDeserializer(TextItem *item)
    : ItemDeserializer(item)
{
}

TextDeserializer::~TextDeserializer() = default;

void TextDeserializer::deserialize(const QJsonObject &obj)
{
    ItemDeserializer::deserialize(obj);
    TextItem *textItem = static_cast<TextItem *>(mItem);
    const QPointF topLeft = toPointF(value(obj, u"top_left"_s));
    textItem->createTextBox(topLeft);
    const QString text = value(obj, u"text"_s).toString();
    if (obj.contains(u"html"_s)) {
        textItem->cursor().insertHtml(value(obj, u"html"_s).toString());
    } else {
        textItem->cursor().insertText(text);
    }
}
