// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "image.hpp"
#include "common/constants.hpp"
#include "common/utils/math.hpp"
#include "serializer/imagedeserializer.hpp"
#include "serializer/imageserializer.hpp"
#include <QJsonObject>
#include <utility>

ImageItem::ImageItem()
{
    m_properties[Property::Type::Opacity] = Property{Common::maxItemOpacity, Property::Type::Opacity};
}

void ImageItem::draw(QPainter &painter, const QPointF &offset)
{
    painter.setOpacity(property(Property::Type::Opacity).value<qreal>() / Common::maxItemOpacity);
    drawItem(painter, offset);
}

void ImageItem::commitTransformation()
{
    const auto [scaleX, scaleY]{Common::Utils::Math::extractScale(m_transform)};
    const QTransform filtered{scaleX, 0, 0, scaleY, 0, 0};

    m_boundingBox = filtered.map(m_boundingBox).boundingRect();

    setDirty(true);
}

bool ImageItem::intersects(const QRectF &rect)
{
    return transformObj().inverted().map(rect).intersects(m_boundingBox);
}

Item::FormType ImageItem::formType() const
{
    return Item::FormType::Image;
}

QJsonObject ImageItem::serialize(int zorder) const
{
    const ImageSerializer serialize(this);
    return serialize.serialize(zorder);
}

void ImageItem::deserialize(const QJsonObject &obj)
{
    ImageDeserializer deserializer(this);
    deserializer.deserialize(obj);
}

void ImageItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    painter.drawPixmap(m_boundingBox.translated(-offset), m_pixmap, m_pixmap.rect());
}

const QPixmap &ImageItem::pixmap() const
{
    return m_pixmap;
}

void ImageItem::setPixmap(QPixmap pixmap)
{
    m_pixmap = std::move(pixmap);
}

QSizeF ImageItem::pastedSize() const
{
    qreal width{static_cast<qreal>(m_pixmap.width())};
    qreal height{static_cast<qreal>(m_pixmap.height())};

    if (std::max(width, height) > Common::pastedImageBoxSize) {
        if (width > height) {
            height = height / width * Common::pastedImageBoxSize;
            width = Common::pastedImageBoxSize;
        } else {
            width = width / height * Common::pastedImageBoxSize;
            height = Common::pastedImageBoxSize;
        }
    }

    return QSizeF{width, height};
}

void ImageItem::setBox(QRectF box)
{
    m_boundingBox = box;
}
