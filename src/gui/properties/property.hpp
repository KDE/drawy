// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "libdrawygui_export.h"
#include <QVariant>
#include <utility>
class QDebug;
class LIBDRAWYGUI_EXPORT Property
{
    Q_GADGET
public:
    Property();

    // They must be in alphabetical order
    // Otherwise, good luck rewriting the autotest files :)
    enum class Type : int8_t {
        Actions,
        Alignment,
        StartArrowType,
        EndArrowType,
        BackgroundColor,
        BackgroundStyle,
        EraserSize,
        FontSize,
        FontStyle,
        Null,
        Opacity,
        StrokeColor,
        StrokeStyle,
        StrokeWidth,
        ZOrder
    };

    enum FontStyle : int8_t {
        Bold = 1 << 0,
        Italic = 1 << 1,
        Underlined = 1 << 2
    };
    Q_ENUM(Type)

    // used to show the widgets in the correct order
    constexpr static int propertyPriority(const Type type)
    {
        switch (type) {
        case Type::StrokeColor:
            return 1;
        case Type::StrokeStyle:
            return 2;
        case Type::BackgroundColor:
            return 3;
        case Type::BackgroundStyle:
            return 4;
        case Type::StrokeWidth:
            return 5;
        case Type::StartArrowType:
            return 6;
        case Type::EndArrowType:
            return 7;
        case Type::Opacity:
            return 8;
        case Type::EraserSize:
            return 9;
        case Type::FontSize:
            return 10;
        case Type::FontStyle:
            return 11;
        case Type::Alignment:
            return 12;
        case Type::ZOrder:
            return 13;
        case Type::Actions:
            return 14;
        case Type::Null:
            return 15;
        }

        return 0;
    }

    template<typename T>
    Property(const T &value, Type type)
        : m_value(value)
        , m_type(type)
    {
    }

    template<typename T>
    void setValue(T value)
    {
        m_value = value;
    }

    template<typename T>
    T value() const
    {
        return m_value.value<T>();
    }

    [[nodiscard]] Type type() const;
    [[nodiscard]] QVariant variant() const;

    [[nodiscard]] static Property::Type convertStringToEnum(const QString &str);
    [[nodiscard]] static QString convertEnumToString(Property::Type type);
    [[nodiscard]] static QString information(Property::Type type);

    [[nodiscard]] bool operator==(const Property &other) const;

private:
    QVariant m_value{};
    Type m_type{};
};
LIBDRAWYGUI_EXPORT QDebug operator<<(QDebug d, const Property &t);
