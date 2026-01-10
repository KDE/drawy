// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "item.hpp"

#include <utility>

#include "common/constants.hpp"
#include "drawy_debug.h"
// PUBLIC
Item::Item()
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

void Item::setProperty(const Property::Type propertyType, Property newObj)
{
    if (m_properties.find(propertyType) != m_properties.end()) {
        m_properties[propertyType] = std::move(newObj);
    }

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

Item::Type Item::convertStringToEnum(const QString &str)
{
    if (str == u"FREEFORM") {
        return Item::Type::Freeform;
    } else if (str == u"RECTANGLE") {
        return Item::Type::Rectangle;
    } else if (str == u"ELLIPSE") {
        return Item::Type::Ellipse;
    } else if (str == u"LINE") {
        return Item::Type::Line;
    } else if (str == u"ARROW") {
        return Item::Type::Arrow;
    } else if (str == u"TEXT") {
        return Item::Type::Text;
    } else if (str == u"GROUP") {
        return Item::Type::Group;
    } else {
        qCWarning(DRAWY_LOG) << "Item::Type is not defined for: " << str;
    }
    return Item::Type::Invalid;
}

QString Item::convertEnumToString(Item::Type type)
{
    switch (type) {
    case Item::Type::Freeform:
        return u"FREEFORM"_s;
    case Item::Type::Rectangle:
        return u"RECTANGLE"_s;
    case Item::Type::Ellipse:
        return u"ELLIPSE"_s;
    case Item::Type::Line:
        return u"LINE"_s;
    case Item::Type::Arrow:
        return u"ARROW"_s;
    case Item::Type::Text:
        return u"TEXT"_s;
    case Item::Type::Group:
        return u"GROUP"_s;
    case Item::Type::Invalid:
        qCWarning(DRAWY_LOG) << "Don't invalid type";
    }
    return {};
}

bool Item::operator==(const Item &other) const
{
    return m_boundingBox == other.m_boundingBox && m_properties == other.m_properties;
}

QDebug operator<<(QDebug d, const Item &t)
{
    d.space() << "boundingBox:" << t.boundingBox();
    d.space() << "properties:" << t.properties();
    return d;
}
#include "moc_item.cpp"
