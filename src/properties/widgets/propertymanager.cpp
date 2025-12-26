// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "propertymanager.hpp"

#include "actionswidget.hpp"
#include "erasersizewidget.hpp"
#include "fontsizewidget.hpp"
#include "strokecolorwidget.hpp"
#include "strokewidthwidget.hpp"

PropertyManager::PropertyManager(QWidget *parent) : QObject{parent} {
    m_widgets[Property::StrokeWidth] = new StrokeWidthWidget(parent);
    m_widgets[Property::StrokeColor] = new StrokeColorWidget(parent);
    m_widgets[Property::EraserSize] = new EraserSizeWidget(parent);
    m_widgets[Property::FontSize] = new FontSizeWidget(parent);
    m_widgets[Property::Actions] = new ActionsWidget(parent);

    for (const auto &[_, widget] : m_widgets) {
        QObject::connect(widget, &PropertyWidget::changed, this, &PropertyManager::propertyUpdated);
    }
}

const PropertyWidget &PropertyManager::widget(const Property::Type type) const {
    if (m_widgets.find(type) == m_widgets.end()) {
        throw std::logic_error("A valid widget for the given Property::Type does not exist.");
    }

    return *m_widgets.at(type);
}

Property PropertyManager::value(const Property::Type type) const {
    return widget(type).value();
}
