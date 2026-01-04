// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QVariant>
#include <utility>

class Property
{
public:
    Property();

    enum class Type : int8_t {
        StrokeWidth,
        StrokeColor,
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

    Type type() const;
    const QVariant variant() const;

private:
    QVariant m_value{};
    Type m_type{};
};
