// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "mime/mimehandler.hpp"

class SvgMimeHandler : public MimeHandler
{
    void contributeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems) override;
};
