// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "image.hpp"
#include "common/constants.hpp"
#include "common/utils/math.hpp"
#include "drawy_gui_intercept_item_debug.h"
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
    painter.setOpacity((qreal)property(Property::Type::Opacity).value<int>() / Common::maxItemOpacity);
    drawItem(painter, offset);
}

void ImageItem::commitTransformation()
{
    const auto [scaleX, scaleY]{Common::Utils::Math::extractScale(m_transform)};
    const QTransform filtered{scaleX, 0, 0, scaleY, 0, 0};

    m_boundingBox = filtered.mapRect(m_boundingBox);

    setDirty(true);
}

bool ImageItem::intersects(const QRectF &rect)
{
    qCDebug(DRAWY_GUI_INTERCEPT_ITEM_LOG) << "rect:" << rect << " m_boundingBox " << m_boundingBox;
    return m_boundingBox.intersects(rect);
}

void ImageItem::translate(const QPointF &amount)
{
    m_boundingBox.translate(amount);
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

void ImageItem::setBox(QRectF box)
{
    m_boundingBox = box;
}
