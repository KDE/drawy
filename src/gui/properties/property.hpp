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

    enum class Type : int8_t {
        Actions,
        Alignment,
        ArrowStyle,
        BackgroundColor,
        BackgroundStyle,
        EraserSize,
        FontSize,
        Null,
        Opacity,
        StrokeColor,
        StrokeStyle,
        StrokeWidth,
        ZOrder
    };

    Q_ENUM(Type)

    template<typename T>
    Property(T value, Type type)
        : m_value(std::move(value))
        , m_type(type)
    {
    }

    template<typename T>
    void setValue(T value)
    {
        m_value = value;
    }

    template<typename T>
    const T value() const
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
