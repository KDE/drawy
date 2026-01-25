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
    ItemDeserializer::deserialize(obj);
    TextItem *textItem = static_cast<TextItem *>(mItem);
    QPointF topLeft = toPointF(value(obj, u"bounding_box"_s));
    const qreal mg{(qreal)value(obj, u"bounding_box_padding"_s).toInt()};
    topLeft += QPointF{mg, mg};
    textItem->createTextBox(topLeft);
    textItem->insertText(value(obj, u"text"_s).toString());
}
