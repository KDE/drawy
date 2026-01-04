// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "property.hpp"

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
