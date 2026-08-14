// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mime/imagemimehandler.hpp"
#include "common/constants.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"
#include "item/image.hpp"
#include "item/item.hpp"
#include <QMimeData>
#include <QUrl>
#include <memory>

ImageMimeHandler::ImageMimeHandler(ApplicationContext *context)
    : m_context(context)
{
}

QList<std::shared_ptr<Item>> ImageMimeHandler::tryReadData(const QMimeData &mimeData)
{
    QPixmap pixmap;

    if (mimeData.hasImage()) {
        pixmap = QPixmap::fromImage(qvariant_cast<QImage>(mimeData.imageData()));
    } else if (!mimeData.urls().isEmpty()) {
        const QUrl url{mimeData.urls().constFirst()};

        if (!url.isLocalFile()) {
            return {};
        }

        if (!pixmap.load(url.toLocalFile())) {
            return {};
        }
    } else {
        return {};
    }

    const std::shared_ptr<ImageItem> image = std::make_shared<ImageItem>();
    image->setPixmap(pixmap);

    const QSizeF boxSize{image->pastedSize() / m_context->renderingContext()->zoomFactor()};

    image->setBox({0, 0, boxSize.width(), boxSize.height()});

    return {image};
}

void ImageMimeHandler::contributeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems)
{
    if (selectedItems.size() != 1 || selectedItems[0]->formType() != Item::FormType::Image) {
        return;
    }

    const ImageItem *item = static_cast<ImageItem *>(&*selectedItems[0]);

    mimeData.setImageData(item->pixmap().toImage());
}
