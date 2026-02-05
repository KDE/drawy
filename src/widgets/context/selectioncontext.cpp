// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "selectioncontext.hpp"

#include "applicationcontext.hpp"
#include "command/commandhistory.hpp"
#include "command/updatepropertycommand.hpp"
#include "coordinatetransformer.hpp"
#include "drawy_debug.h"
#include "item/item.hpp"
#include "properties/property.hpp"
#include "renderingcontext.hpp"
#include "spatialcontext.hpp"

SelectionContext::SelectionContext(ApplicationContext *context)
    : QObject{context}
    , m_applicationContext{context}
{
}

SelectionContext::~SelectionContext()
{
    qCDebug(DRAWY_LOG) << "Object deleted: SelectionContext";
}

const std::unordered_set<std::shared_ptr<Item>> &SelectionContext::selectedItems() const
{
    return m_selectedItems;
}

void SelectionContext::addToSelection(const std::shared_ptr<Item> &item)
{
    if (m_selectedItems.contains(item)) {
        return;
    }

    m_selectedItems.insert(item);
    Q_EMIT selectionUpdated();
}

void SelectionContext::removeFromSelection(const std::shared_ptr<Item> &item)
{
    if (!m_selectedItems.contains(item)) {
        return;
    }

    m_selectedItems.erase(item);
    Q_EMIT selectionUpdated();
}

QRectF SelectionContext::selectionBox() const
{
    QRectF selectionBox;
    for (const auto &item : m_selectedItems) {
        selectionBox |= item->boundingBox();
    }
    return selectionBox;
}

// PUBLIC SLOTS
void SelectionContext::updatePropertyOfSelectedItems(const Property &property)
{
    const QList<std::shared_ptr<Item>> items{m_selectedItems.begin(), m_selectedItems.end()};

    const auto commandHistory{m_applicationContext->spatialContext()->commandHistory()};
    commandHistory->insert(std::make_shared<UpdatePropertyCommand>(items, property));

    m_applicationContext->renderingContext()->markForRender();
    m_applicationContext->renderingContext()->markForUpdate();
}

void SelectionContext::reset()
{
    if (m_selectedItems.empty()) {
        return;
    }

    m_selectedItems.clear();
    Q_EMIT selectionUpdated();
}

#include "moc_selectioncontext.cpp"
