// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "libdrawywidgets_private_export.h"
#include <QVariant>
#include <utility>
class LIBDRAWYWIDGETS_TESTS_EXPORT Property
{
public:
    Property();

    enum class Type : int8_t {
        StrokeWidth,
        StrokeColor,
        // StrokeType, // We need to define several stroke
        Opacity,
        FontSize,
        EraserSize,
        Actions,
        Null
    };

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
    const QVariant variant() const;

    [[nodiscard]] Property::Type convertStringToEnum(const QString &str) const;
    [[nodiscard]] QString convertStringToEnum(Property::Type type) const;

private:
    QVariant m_value{};
    Type m_type{};
};
