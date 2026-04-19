// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "mime/mimehandler.hpp"

class ApplicationContext;

class ImageMimeHandler : public MimeHandler
{
public:
    explicit ImageMimeHandler(ApplicationContext *context);

    [[nodiscard]] QList<std::shared_ptr<Item>> tryReadData(const QMimeData &mimeData) override;

    void contributeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems) override;

private:
    ApplicationContext *const m_context;
};
