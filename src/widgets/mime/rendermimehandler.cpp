// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mime/rendermimehandler.hpp"
#include "common/constants.hpp"
#include "item/item.hpp"
#include <QByteArray>
#include <QImage>
#include <QMimeData>
#include <QPainter>
#include <memory>

using namespace Qt::StringLiterals;

void RenderMimeHandler::contributeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems)
{
    QRectF boundingBox;

    for (const auto &item : std::as_const(selectedItems)) {
        boundingBox |= item->boundingBox();
    }

    QImage image(boundingBox.size().toSize(), QImage::Format_ARGB32);
    image.fill(Common::darkBackgroundColor);

    QPainter painter(&image);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    for (const auto &item : std::as_const(selectedItems)) {
        painter.save();
        item->draw(painter, boundingBox.topLeft());
        painter.restore();
    }

    mimeData.setImageData(image);
}
