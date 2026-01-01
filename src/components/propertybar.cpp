// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "propertybar.hpp"

#include <QLabel>
#include <QSpacerItem>
#include <stdexcept>

#include "../context/applicationcontext.hpp"
#include "../context/uicontext.hpp"
#include "../properties/widgets/propertymanager.hpp"
#include "../properties/widgets/propertywidget.hpp"
#include "../tools/tool.hpp"
#include "toolbar.hpp"

PropertyBar::PropertyBar(QWidget *parent)
    : QFrame{parent}
    , m_layout(new QVBoxLayout{this})
{
    this->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    this->setAutoFillBackground(true);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->setProperty("class", "drawlyFrame drawyPropertyBar");
}

void PropertyBar::setPropertyManager(PropertyManager *manager)
{
    m_propertyManager = manager;
}

void PropertyBar::updateToolProperties()
{
    ApplicationContext *context{ApplicationContext::instance()};
    updateProperties(context->uiContext()->toolBar()->curTool());
}

// PUBLIC SLOTS
void PropertyBar::updateProperties(const Tool &tool)
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

    QVector<Property::Type> properties{tool.properties()};
    if (properties.empty()) {
        hide();
    } else {
        show();
    }

    qsizetype count{0};
    for (Property::Type property : properties) {
        try {
            const PropertyWidget &widget{m_propertyManager->widget(property)};
            QLabel *widgetLabel{new QLabel{widget.name(), this}};
            m_layout->addWidget(widgetLabel);
            m_layout->addWidget(widget.widget());

            if (count > 0) {
                widgetLabel->setProperty("class", "drawlyPropertyLabelMargin");
            }

            count++;
            widget.widget()->show();
        } catch (const std::logic_error &e) {
            // ignore this property
        }
    }

    update();
}

#include "moc_propertybar.cpp"
