// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "item.hpp"
#include <QPainterPath>
#include <QUuid>

#include "common/constants.hpp"
#include "drawy_gui_debug.h"
#include "item/itemutils.hpp"
#include <utility>

// PUBLIC
Item::Item()
    : m_id(QUuid::createUuid().toByteArray(QUuid::Id128))
{
}

Item::~Item()
{
    qCDebug(DRAWY_GUI_LOG) << "Item deleted: " << m_boundingBox;
}

QRectF Item::boundingBox() const
{
    return m_transform.map(m_boundingBox).boundingRect();
}

QRectF Item::normalizedBoundingBox() const
{
    return m_boundingBox.normalized();
}

QPolygonF Item::displayBoundingBox() const
{
    return m_transform.map(m_boundingBox.normalized());
}

void Item::translate(const QPointF &amount)
{
    m_transform.translate(amount.x(), amount.y());
}

void Item::rotate(const qreal angleDeg, const QPointF pivot)
{
    m_transform.translate(pivot.x(), pivot.y());
    m_transform.rotate(angleDeg);
    m_transform.translate(-pivot.x(), -pivot.y());
}

void Item::resize(const QTransform operation)
{
    m_transform = m_transform * operation;
}

bool Item::isFilled() const
{
    if (!hasProperty(Property::Type::BackgroundColor)) {
        return false;
    }
    if (hasProperty(Property::Type::BackgroundStyle)
        && ItemUtils::convertBackgroundTypeStringToEnum(property(Property::Type::BackgroundStyle).value<QString>()) == Item::BackgroundType::None) {
        return true;
    }
    return property(Property::Type::BackgroundColor).value<QColor>().alpha() != 0;
}

Property Item::property(const Property::Type propertyType) const
{
    if (!m_properties.contains(propertyType)) {
        throw std::logic_error("Item does not support this property.");
    }

    return m_properties.at(propertyType);
}

QList<Property::Type> Item::propertyTypes() const
{
    QList<Property::Type> result;

    for (auto &[formType, _] : m_properties) {
        result.push_back(formType);
    }

    return result;
}

QList<Property> Item::properties() const
{
    QList<Property> result;

    for (auto &[_, property] : m_properties) {
        result.push_back(property);
    }

    return result;
}

bool Item::hasProperty(Property::Type propertyType) const
{
    return m_properties.contains(propertyType);
}

QByteArray Item::id() const
{
    return m_id;
}

void Item::setId(const QByteArray &newId)
{
    m_id = newId;
}

bool Item::locked() const
{
    return m_locked;
}

void Item::setLocked(bool newLocked)
{
    m_locked = newLocked;
}

void Item::setProperty(const Property::Type propertyType, Property newObj)
{
    if (m_properties.contains(propertyType)) {
        m_properties[propertyType] = std::move(newObj);
    }

    setDirty(true);
    updateAfterProperty();
}

void Item::updateAfterProperty()
{
}

void Item::erase([[maybe_unused]] QPainter &painter, [[maybe_unused]] const QPointF &offset) const
{
    painter.save();
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(boundingBox().translated(-offset), Qt::transparent);
    painter.restore();
}

bool Item::operator==(const Item &other) const
{
    return m_boundingBox == other.m_boundingBox && m_properties == other.m_properties && m_id == other.m_id && m_locked == other.m_locked;
}

QDebug operator<<(QDebug d, const Item &t)
{
    d.space() << "boundingBox:" << t.boundingBox();
    d.space() << "properties:" << t.properties();
    d.space() << "id:" << t.id();
    d.space() << "locked:" << t.locked();
    return d;
}

const QTransform &Item::transformObj() const
{
    return m_transform;
}

void Item::setTransform(const QTransform &transformObj)
{
    m_transform = transformObj;
}

QList<TransformHandlerUtils::Type> Item::transformHandlers() const
{
    using enum TransformHandlerUtils::Type;
    return {ResizeTransformHandler, MoveTransformHandler, RotateTransformHandler};
}

// reimplement this if your item can benefit from caching
bool Item::needsCaching() const
{
    return false;
}

bool Item::isDirty() const
{
    return m_isDirty;
}

void Item::setDirty(bool value)
{
    m_isDirty = value;
}

bool Item::lockAspectRatioWhenResizing() const
{
    return false;
}

#include "moc_item.cpp"
