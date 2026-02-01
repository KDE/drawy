/*
 * SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "imageserializer.hpp"
#include <QBuffer>
#include <QByteArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

ImageSerializer::ImageSerializer(const ImageItem *item)
    : ItemSerializer(item)
{
}

ImageSerializer::~ImageSerializer() = default;

QJsonObject ImageSerializer::serialize(int zorder) const
{
    QJsonObject obj = ItemSerializer::serialize(zorder);
    const ImageItem *img = static_cast<const ImageItem *>(mItem);

    QByteArray data;
    QBuffer dataBuf(&data);
    img->pixmap().save(&dataBuf, "PNG");

    obj[u"image_data"_s] = QJsonValue(QString::fromUtf8(data.toBase64()));
    obj[u"bounding_box"_s] = ItemSerializer::toJson(img->normalizedBoundingBox());
    return obj;
}
