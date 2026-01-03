// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "selectioncontext.hpp"

#include "applicationcontext.hpp"
#include "command/commandhistory.hpp"
#include "command/updatepropertycommand.hpp"
#include "coordinatetransformer.hpp"
#include "data-structures/cachegrid.hpp"
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

std::unordered_set<std::shared_ptr<Item>> &SelectionContext::selectedItems()
{
    return m_selectedItems;
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
    QVector<std::shared_ptr<Item>> items{m_selectedItems.begin(), m_selectedItems.end()};

    auto commandHistory{m_applicationContext->spatialContext()->commandHistory()};
    commandHistory->insert(std::make_shared<UpdatePropertyCommand>(items, property));

    m_applicationContext->renderingContext()->markForRender();
    m_applicationContext->renderingContext()->markForUpdate();
}

void SelectionContext::reset()
{
    selectedItems().clear();
}
