// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mime/textmimehandler.hpp"
#include "common/constants.hpp"
#include "item/item.hpp"
#include "item/itemutils.hpp"
#include "item/text.hpp"
#include "serializer/itemdeserializer.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMimeData>
#include <memory>
#include <qjsondocument.h>

QList<std::shared_ptr<Item>> TextMimeHandler::tryReadData(const QMimeData &mimeData)
{
    if (!mimeData.hasText()) {
        return {};
    }

    if (mimeData.text().trimmed().isEmpty()) {
        return {};
    }

    std::shared_ptr<TextItem> text = std::make_shared<TextItem>();

    text->createTextBox({0, 0});
    text->insertText(mimeData.text());

    return {text};
}

void TextMimeHandler::contributeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems)
{
    if (selectedItems.size() != 1 || selectedItems[0]->formType() != Item::FormType::Text) {
        return;
    }

    TextItem *item = static_cast<TextItem *>(&*selectedItems[0]);

    mimeData.setText(item->text());
}
