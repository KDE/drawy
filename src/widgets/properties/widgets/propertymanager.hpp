// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawywidgets_private_export.h"
#include "properties/property.hpp"
#include "properties/widgets/propertywidget.hpp"
#include <QObject>

class LIBDRAWYWIDGETS_TESTS_EXPORT PropertyManager : public QObject
{
    Q_OBJECT
public:
    explicit PropertyManager(QWidget *parent = nullptr);
    ~PropertyManager() override;

    [[nodiscard]] PropertyWidget *widget(const Property::Type type) const;
    [[nodiscard]] Property value(const Property::Type type) const;

Q_SIGNALS:
    void propertyUpdated(Property property);

private:
    QMap<Property::Type, PropertyWidget *> m_widgets{};
};
