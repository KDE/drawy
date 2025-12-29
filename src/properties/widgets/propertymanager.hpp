// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>

#include "../property.hpp"
#include "propertywidget.hpp"

class PropertyManager : public QObject
{
    Q_OBJECT
public:
    explicit PropertyManager(QWidget *parent = nullptr);

    const PropertyWidget &widget(const Property::Type type) const;
    Property value(const Property::Type type) const;

Q_SIGNALS:
    void propertyUpdated(Property property);

private:
    std::unordered_map<Property::Type, PropertyWidget *> m_widgets{};
};
