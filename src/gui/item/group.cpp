// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "group.hpp"
#include "common/utils/math.hpp"
#include "serializer/groupdeserializer.hpp"
#include "serializer/groupserializer.hpp"

#include <QJsonObject>

void GroupItem::draw(QPainter &painter, const QPointF &offset)
{
    for (const auto &item : std::as_const(m_items)) {
        painter.save();
        painter.setTransform(item->transformObj(), true);
        item->draw(painter, offset);
        painter.restore();
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
    Item::translate(amount);
}

void GroupItem::commitTransformation()
{
    const auto [scaleX, scaleY]{Common::Utils::Math::extractScale(m_transform)};
    const QTransform filtered{scaleX, 0, 0, scaleY, 0, 0};

    for (auto &item : std::as_const(m_items)) {
        const QTransform combined{item->transformObj() * filtered};

        item->setTransform(combined);
        item->commitTransformation();
        item->setDirty(true);
    }

    setDirty(true);
}

void GroupItem::group(const QList<std::shared_ptr<Item>> &items)
{
    m_items = items;

    QRectF unitedBoundingBox{};
    for (const auto &item : std::as_const(m_items)) {
        unitedBoundingBox |= item->boundingBox();
    }

    translate(unitedBoundingBox.topLeft());

    for (auto &item : std::as_const(m_items)) {
        const QPointF globalOffset{-unitedBoundingBox.topLeft()};
        const QTransform t{item->transformObj()};
        const QPointF localOffset{t.inverted().map(globalOffset) - t.inverted().map(QPointF(0, 0))};
        item->translate(localOffset);
    }
}

bool GroupItem::intersects(const QRectF &rect)
{
    return std::ranges::any_of(m_items, [this, rect](const auto &item) {
        return item->intersects(m_transform.inverted().map(rect).boundingRect());
    });
}

QList<std::shared_ptr<Item>> GroupItem::unGroup()
{
    const QTransform groupTransform{transformObj()};
    for (auto &item : std::as_const(m_items)) {
        const QTransform combined{item->transformObj() * groupTransform};

        item->setTransform(combined);
        item->commitTransformation();
        item->setDirty(true);
    }

    setDirty(true);
    return m_items;
}

QRectF GroupItem::boundingBox() const
{
    return m_transform.map(normalizedBoundingBox()).boundingRect();
}

QRectF GroupItem::normalizedBoundingBox() const
{
    QRectF result;

    for (const auto &item : m_items) {
        result |= item->boundingBox();
    }

    return result;
}

QPolygonF GroupItem::displayBoundingBox() const
{
    return m_transform.map(normalizedBoundingBox());
}

Item::FormType GroupItem::formType() const
{
    return Item::FormType::Group;
}

void GroupItem::setProperty(const Property::Type propertyType, Property newObj)
{
    if (newObj.variant().typeId() == QMetaType::QVariantHash) {
        const QVariantHash variant = newObj.variant().toHash();
        for (const auto &item : std::as_const(m_items)) {
            if (variant.contains(QString::fromUtf8(item->id()))) {
                item->setProperty(propertyType, Property(variant[QString::fromUtf8(item->id())], propertyType));
            }
        }
        return;
    }

    for (const auto &item : std::as_const(m_items)) {
        item->setProperty(propertyType, newObj);
    }

    setDirty(true);
}

Property GroupItem::property(const Property::Type propertyType) const
{
    if (m_items.empty()) {
        throw std::logic_error("Group does not contain any item with this property");
    }

    Property property;
    for (const auto &item : m_items) {
        if (!item->hasProperty(propertyType)) {
            continue;
        }

        if (property.type() != Property::Type::Null) {
            if (property.variant() != item->property(propertyType).variant()) {
                QVariantHash itemVariant;
                for (const auto &child : m_items) {
                    if (child->hasProperty(propertyType)) {
                        itemVariant.insert(QString::fromUtf8(child->id()), child->property(propertyType).variant());
                    }
                }
                return Property(itemVariant, propertyType);
            }
        } else {
            property = item->property(propertyType);
        }
    }

    return property;
}

QList<Property> GroupItem::properties() const
{
    QList<Property> result;
    auto types = propertyTypes();
    std::sort(types.begin(), types.end());
    for (const auto type : std::as_const(types)) {
        const Property prop = property(type);
        if (prop.type() != Property::Type::Null) {
            result.append(prop);
        }
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

QList<Property::Type> GroupItem::allPropertyTypes() const
{
    std::unordered_set<Property::Type> types;
    for (const auto &item : m_items) {
        const auto itemPropertyTypes{item->allPropertyTypes()};
        types.insert(itemPropertyTypes.begin(), itemPropertyTypes.end());
    }

    return QList<Property::Type>(types.begin(), types.end());
}

bool GroupItem::hasProperty(Property::Type propertyType) const
{
    return std::ranges::any_of(m_items, [propertyType](const auto &item) {
        return item->hasProperty(propertyType);
    });
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
    return std::ranges::any_of(m_items, [](const auto &item) {
        return item->needsCaching();
    });
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

bool GroupItem::lockAspectRatioWhenResizing() const
{
    return true;
}
