// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mime/mimehandler.hpp"

MimeHandler::~MimeHandler() = default;

QList<std::shared_ptr<Item>> MimeHandler::tryReadData([[maybe_unused]] const QMimeData &mimeData)
{
    return {};
}

void MimeHandler::contributeData([[maybe_unused]] QMimeData &mimeData, [[maybe_unused]] const QList<std::shared_ptr<Item>> &selectedItems)
{
}
