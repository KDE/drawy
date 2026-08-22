/*
 * SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "imagedeserializer.hpp"
#include <QJsonObject>
#include <utility>

using namespace Qt::Literals::StringLiterals;

ImageDeserializer::ImageDeserializer(ImageItem *item)
    : ItemDeserializer(item)
{
}

ImageDeserializer::~ImageDeserializer() = default;

void ImageDeserializer::deserialize(const QJsonObject &obj)
{
    ItemDeserializer::deserialize(obj);

    ImageItem *image = static_cast<ImageItem *>(mItem);

    const QRectF box = toRectF(value(obj, u"bounding_box"_s));
    image->setBox(box);

    const QByteArray data{QByteArray::fromBase64(value(obj, u"image_data"_s).toString().toUtf8())};
    QPixmap pixmap;
    pixmap.loadFromData(data, "PNG");
    image->setPixmap(std::move(pixmap));
}
