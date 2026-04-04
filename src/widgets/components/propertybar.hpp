// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "frame.hpp"
#include "properties/property.hpp"
#include <QVBoxLayout>

class Tool;
class PropertyManager;
class PropertyWidget;
class ApplicationContext;

class PropertyBar : public Frame
{
    Q_OBJECT
public:
    explicit PropertyBar(ApplicationContext *context, QWidget *parent = nullptr);

    void setPropertyManager(PropertyManager *manager);
    void updateToolProperties();
    void updateProperties(Tool &);

    [[nodiscard]] QSize sizeHint() const override;

private:
    void assignPropertyValue(Property::Type property, PropertyWidget *widget);

    QWidget *m_contentWidget = nullptr; // Added container widget
    QVBoxLayout *const m_layout;
    PropertyManager *m_propertyManager = nullptr;
    ApplicationContext *const m_context;

    bool m_updateInProgress{false};
};
