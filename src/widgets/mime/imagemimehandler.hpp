// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "mime/mimehandler.hpp"

class ImageMimeHandler : public MimeHandler
{
    [[nodiscard]] QList<std::shared_ptr<Item>> tryReadData(const QMimeData &mimeData) override;

    void contributeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems) override;
};
