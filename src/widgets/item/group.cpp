// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "group.hpp"
#include "serializer/groupdeserializer.hpp"
#include "serializer/groupserializer.hpp"

#include <QJsonObject>
#include <stdexcept>

void GroupItem::draw(QPainter &painter, const QPointF &offset)
{
    for (const auto &item : std::as_const(m_items)) {
        item->draw(painter, offset);
    }
}

void GroupItem::erase(QPainter &painter, const QPointF &offset) const
{
    for (const auto &item : m_items) {
        item->erase(painter, offset);
    }
}

void GroupItem::translate(const QPointF &amount)
{
    for (const auto &item : std::as_const(m_items)) {
        item->translate(amount);
    }
}

void GroupItem::group(const QList<std::shared_ptr<Item>> &items)
{
    m_items = items;
}

bool GroupItem::intersects(const QRectF &rect)
{
    for (const auto &item : std::as_const(m_items)) {
        if (item->intersects(rect)) {
            return true;
        }
    }

    return false;
}

QList<std::shared_ptr<Item>> GroupItem::unGroup()
{
    setDirty(true);
    return m_items;
}

QRectF GroupItem::boundingBox() const
{
    QRectF result;

    for (const auto &item : m_items) {
        result |= item->boundingBox();
    }

    return result;
}

Item::FormType GroupItem::formType() const
{
    return Item::FormType::Group;
}

void GroupItem::setProperty(const Property::Type propertyType, Property newObj)
{
    for (const auto &item : std::as_const(m_items)) {
        item->setProperty(propertyType, newObj);
    }
}

Property GroupItem::property(const Property::Type propertyType) const
{
    if (m_items.empty()) {
        throw new std::logic_error("Group does not contain any item with this property");
    }

    Property property;
    for (const auto &item : m_items) {
        try {
            if (property.type() != Property::Type::Null) {
                if (property.variant() != item->property(propertyType).variant()) {
                    return Property{};
                }
            } else {
                property = item->property(propertyType);
            }
        } catch (const std::logic_error &) {
            // ignore
        }
    }

    return property;
}

QList<Property> GroupItem::properties() const
{
    QList<Property> result;

    for (const auto &item : m_items) {
        result += item->properties();
    }

    return result;
}

QList<Property::Type> GroupItem::propertyTypes() const
{
    std::unordered_set<Property::Type> types;
    for (const auto &item : m_items) {
        const auto itemPropertyTypes{item->propertyTypes()};
        types.insert(itemPropertyTypes.begin(), itemPropertyTypes.end());
    }

    return QList<Property::Type>(types.begin(), types.end());
}

void GroupItem::drawItem([[maybe_unused]] QPainter &painter, [[maybe_unused]] const QPointF &offset) const
{
}

QJsonObject GroupItem::serialize(int zorder) const
{
    const GroupSerializer serializer(this);
    return serializer.serialize(zorder);
}

bool GroupItem::needsCaching() const
{
    return true;
}

QList<std::shared_ptr<Item>> GroupItem::items() const
{
    return m_items;
}

void GroupItem::setItems(const QList<std::shared_ptr<Item>> &newItems)
{
    m_items = newItems;
}

void GroupItem::deserialize(const QJsonObject &obj)
{
    GroupDeserializer deserializer(this);
    deserializer.deserialize(obj);
}
