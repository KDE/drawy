// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "property.hpp"
#include "drawy_debug.h"
using namespace Qt::Literals::StringLiterals;
Property::Property()
    : m_value{0}
    , m_type{Property::Type::Null}
{
}

Property::Type Property::type() const
{
    return m_type;
}

const QVariant Property::variant() const
{
    return m_value;
}

Property::Type Property::convertStringToEnum(const QString &str) const
{
    if (str == u"StrokeWidth") {
        return Property::Type::StrokeWidth;
    } else if (str == u"StrokeColor") {
        return Property::Type::StrokeColor;
    } else if (str == u"Opacity") {
        return Property::Type::Opacity;
    } else if (str == u"FontSize") {
        return Property::Type::FontSize;
    } else if (str == u"EraserSize") {
        return Property::Type::EraserSize;
    } else if (str == u"Actions") {
        return Property::Type::Actions;
    } else {
        qCWarning(DRAWY_LOG) << "Property::Type is not defined for: " << str;
    }
    return Property::Type::Null;
}

QString Property::convertStringToEnum(Property::Type type) const
{
    switch (type) {
    case Property::Type::StrokeWidth:
        return u"StrokeWidth"_s;
    case Property::Type::StrokeColor:
        return u"StrokeColor"_s;
    case Property::Type::Opacity:
        return u"Opacity"_s;
    case Property::Type::FontSize:
        return u"FontSize"_s;
    case Property::Type::EraserSize:
        return u"EraserSize"_s;
    case Property::Type::Actions:
        return u"Actions"_s;
    case Property::Type::Null:
        qCWarning(DRAWY_LOG) << "Don't save null property ";
    }
    return {};
}
