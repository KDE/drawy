// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mime/textmimehandler.hpp"
#include "item/item.hpp"
#include "item/text.hpp"
#include <QMimeData>
#include <memory>

QList<std::shared_ptr<Item>> TextMimeHandler::tryReadData(const QMimeData &mimeData)
{
    if (mimeData.hasHtml() && !mimeData.html().trimmed().isEmpty()) {
        const auto text = std::make_shared<TextItem>();
        text->createTextBox({0, 0});
        text->cursor().insertHtml(mimeData.html());
        return {text};
    }

    if (mimeData.hasText() && !mimeData.text().trimmed().isEmpty()) {
        const auto text = std::make_shared<TextItem>();
        text->createTextBox({0, 0});
        text->cursor().insertText(mimeData.text());
        return {text};
    }

    return {};
}

void TextMimeHandler::contributeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems)
{
    if (selectedItems.size() != 1 || selectedItems[0]->formType() != Item::FormType::Text) {
        return;
    }

    const TextItem *item = static_cast<TextItem *>(&*selectedItems[0]);

    mimeData.setText(item->text());
    mimeData.setHtml(item->html());
}
