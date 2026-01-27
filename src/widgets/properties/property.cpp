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

Property::Type Property::convertStringToEnum(const QString &str)
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
    } else if (str == u"StrokeStyle") {
        return Property::Type::StrokeStyle;
    } else if (str == u"BackgroundColor") {
        return Property::Type::BackgroundColor;
    } else if (str == u"BackgroundStyle") {
        return Property::Type::BackgroundStyle;
    } else if (str == u"ZOrder") {
        return Property::Type::ZOrder;
    } else {
        qCWarning(DRAWY_LOG) << "Property::Type is not defined for: " << str;
    }
    return Property::Type::Null;
}

QString Property::convertEnumToString(Property::Type type)
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
    case Property::Type::StrokeStyle:
        return u"StrokeStyle"_s;
    case Property::Type::BackgroundColor:
        return u"BackgroundColor"_s;
    case Property::Type::BackgroundStyle:
        return u"BackgroundStyle"_s;
    case Property::Type::ArrowStyle:
        // We don't store Arrow in XML
        return {};
    case Property::Type::Alignment:
        // We don't store Alignment in XML
        return {};
    case Property::Type::ZOrder:
        return u"ZOrder"_s;
    case Property::Type::Null:
        qCWarning(DRAWY_LOG) << "Don't save null property ";
    }
    return {};
}

QString Property::information(Type type)
{
    switch (type) {
    case Property::Type::StrokeWidth:
        return QObject::tr("Change Stroke Width");
    case Property::Type::StrokeColor:
        return QObject::tr("Change Stroke Color");
    case Property::Type::Opacity:
        return QObject::tr("Change Opacity");
    case Property::Type::FontSize:
        return QObject::tr("Change Font Size");
    case Property::Type::EraserSize:
        return QObject::tr("Change Eraser Size");
    case Property::Type::Actions:
        return QObject::tr("Actions");
    case Property::Type::StrokeStyle:
        return QObject::tr("Change Stroke Style");
    case Property::Type::BackgroundColor:
        return QObject::tr("Change Background Color");
    case Property::Type::BackgroundStyle:
        return QObject::tr("Change Background Style");
    case Property::Type::ArrowStyle:
        // We don't store Arrow in XML
        return {};
    case Property::Type::Alignment:
        // We don't store Alignment in XML
        return {};
    case Property::Type::ZOrder:
        return QObject::tr("Change ZOrder");
    case Property::Type::Null:
        qCWarning(DRAWY_LOG) << "Don't save null property ";
    }
    return {};
}

bool Property::operator==(const Property &other) const
{
    return m_value == other.m_value && m_type == other.m_type;
}

QDebug operator<<(QDebug d, const Property &t)
{
    d.space() << "value:" << t.variant();
    d.space() << "type:" << t.type();
    return d;
}
#include "moc_property.cpp"
