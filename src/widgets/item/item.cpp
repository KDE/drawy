// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "item.hpp"
#include <QUuid>

#include <utility>

#include "common/constants.hpp"
#include "drawy_debug.h"
// PUBLIC
Item::Item()
    : m_id(QUuid::createUuid().toByteArray(QUuid::Id128))
{
}

Item::~Item()
{
    qCDebug(DRAWY_LOG) << "Item deleted: " << m_boundingBox;
}

const QRectF Item::boundingBox() const
{
    int mg{boundingBoxPadding()};
    return m_boundingBox.adjusted(-mg, -mg, mg, mg);
}

const Property Item::property(const Property::Type propertyType) const
{
    if (m_properties.find(propertyType) == m_properties.end()) {
        throw std::logic_error("Item does not support this property.");
    }

    return m_properties.at(propertyType);
}

const QList<Property::Type> Item::propertyTypes() const
{
    QList<Property::Type> result;

    for (auto &[type, _] : m_properties) {
        result.push_back(type);
    }

    return result;
}

const QList<Property> Item::properties() const
{
    QList<Property> result;

    for (auto &[_, property] : m_properties) {
        result.push_back(property);
    }

    return result;
}

bool Item::hasProperty(Property::Type propertyType) const
{
    return m_properties.find(propertyType) != m_properties.end();
}

QByteArray Item::id() const
{
    return m_id;
}

void Item::setId(const QByteArray &newId)
{
    m_id = newId;
}

void Item::setProperty(const Property::Type propertyType, Property newObj)
{
    if (m_properties.find(propertyType) != m_properties.end()) {
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
}

int Item::boundingBoxPadding() const
{
    return Common::boundingBoxPadding;
}

bool Item::operator==(const Item &other) const
{
    return m_boundingBox == other.m_boundingBox && m_properties == other.m_properties && m_id == other.m_id;
}

QDebug operator<<(QDebug d, const Item &t)
{
    d.space() << "boundingBox:" << t.boundingBox();
    d.space() << "properties:" << t.properties();
    d.space() << "id:" << t.id();
    return d;
}

const QTransform &Item::transformObj() const
{
    return m_transform;
}

// reimplement this if your item can benefit from caching
bool Item::needsCaching() const
{
    return false;
}

bool Item::isDirty() const
{
    return m_isDirty;
};

void Item::setDirty(bool value)
{
    m_isDirty = value;
}

#include "moc_item.cpp"
