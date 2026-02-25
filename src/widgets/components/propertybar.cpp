// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "propertybar.hpp"

#include <QLabel>
#include <QStyle>
#include <stdexcept>

#include "context/applicationcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/uicontext.hpp"
#include "item/item.hpp"
#include "properties/widgets/propertymanager.hpp"
#include "properties/widgets/propertywidget.hpp"
#include "toolbar.hpp"
#include "tools/tool.hpp"
using namespace Qt::Literals::StringLiterals;

PropertyBar::PropertyBar(ApplicationContext *context, QWidget *parent)
    : Frame{parent}
    , m_layout(new QVBoxLayout{this})
    , m_context(context)
{
    const int margins{style()->pixelMetric(QStyle::PM_ToolBarItemMargin) * 2};
    m_layout->setContentsMargins(margins, margins, margins, margins);
}

void PropertyBar::setPropertyManager(PropertyManager *manager)
{
    m_propertyManager = manager;
}

void PropertyBar::updateToolProperties()
{
    updateProperties(m_context->uiContext()->toolBar()->curTool());
}

void PropertyBar::updateProperties(Tool &tool)
{
    // remove existing widgets
    QLayoutItem *curItem = nullptr;
    while ((curItem = m_layout->takeAt(0)) != nullptr) {
        QWidget *widget = curItem->widget();
        if (widget) {
            widget->hide();
            widget->setParent(nullptr);

            // this is the property label
            if (qobject_cast<QLabel *>(widget) != nullptr) {
                widget->deleteLater();
            }
        }
        delete curItem;
    }

    const QList<Property::Type> properties{tool.properties()};
    if (properties.empty()) {
        hide();
    } else {
        show();
    }

    for (Property::Type property : properties) {
        try {
            PropertyWidget *const widget{m_propertyManager->widget(property)};
            auto *widgetLabel{new QLabel{widget->name(), this}};
            m_layout->addWidget(widgetLabel);
            m_layout->addWidget(widget->widget());

            widget->widget()->show();
            assignPropertyValue(property, widget);
        } catch (const std::logic_error &) {
            // ignore this property
        }
    }

    update();
}

void PropertyBar::assignPropertyValue(Property::Type property, PropertyWidget *widget)
{
    const auto &selectedItems{m_context->selectionContext()->selectedItems()};
    QVariant propertyValue;
    for (const auto &item : selectedItems) {
        if (item->hasProperty(property)) {
            if (propertyValue.isValid() && (propertyValue != item->property(property).variant())) {
                return;
            }
            propertyValue = item->property(property).variant();
        }
    }
    if (propertyValue.isValid()) {
        widget->setValue(propertyValue);
    }
}

#include "moc_propertybar.cpp"
