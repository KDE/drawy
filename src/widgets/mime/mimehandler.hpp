// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QList>
#include <memory>

class Item;
class QMimeData;

// This class represents anything that might contribute to the handling of the
// clipboard or drag and drop.
class MimeHandler
{
public:
    virtual ~MimeHandler();

    // Called to read items from a QMimeData, used when dropping or pasting.
    // If the returned list is not empty, the operation is considered to have been
    // handled.
    [[nodiscard]] virtual QList<std::shared_ptr<Item>> tryReadData(const QMimeData &mimeData);

    // Called to contribute formats to a QMimeData, used when dragging or copying.
    virtual void contributeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems);
};
