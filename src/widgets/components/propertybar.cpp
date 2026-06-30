// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "propertybar.hpp"

#include <QLabel>
#include <QScrollArea>
#include <QStyle>

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
    , m_contentWidget{new QWidget(this)}
    , m_layout{new QVBoxLayout(m_contentWidget)}
    , m_context{context}
{
    const int margins{style()->pixelMetric(QStyle::PM_ToolBarItemMargin) * 2};
    m_layout->setContentsMargins(margins, margins, margins, margins);

    m_layout->setAlignment(Qt::AlignTop);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(m_contentWidget);

    scrollArea->setWidgetResizable(true);

    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    scrollArea->setFrameShape(QFrame::NoFrame);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(scrollArea);
}

QSize PropertyBar::sizeHint() const
{
    return m_contentWidget->sizeHint();
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
    if (m_updateInProgress) {
        return;
    }

    m_updateInProgress = true;

    hide();

    // remove existing widgets
    const QLayoutItem *curItem = nullptr;
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

    QList<Property::Type> properties{tool.properties()};

    std::sort(properties.begin(), properties.end(), [](const auto first, const auto second) -> bool {
        return Property::propertyPriority(first) < Property::propertyPriority(second);
    });

    for (const Property::Type property : properties) {
        try {
            PropertyWidget *const widget{m_propertyManager->widget(property)};

            auto widgetLabel{new QLabel{widget->name(), m_contentWidget}};

            m_layout->addWidget(widgetLabel);
            m_layout->addWidget(widget->widget());

            widget->widget()->show();
            assignPropertyValue(property, widget);
        } catch (const std::logic_error &) { // NOLINT(bugprone-empty-catch)
            // ignore since the item does not support this property
        }
    }

    if (properties.empty()) {
        hide();
    } else {
        show();
    }

    update();

    m_updateInProgress = false;
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
