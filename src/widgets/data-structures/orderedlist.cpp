// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "orderedlist.hpp"

#include "drawy_debug.h"

OrderedList::~OrderedList()
{
    qCDebug(DRAWY_LOG) << "Object deleted: OrderedList";
}

bool OrderedList::hasItem(const ItemPtr &item) const
{
    if (!m_itemIterMap.contains(item)) {
        return false;
    }
    if (!m_zIndex.contains(item)) {
        return false;
    }
    return true;
}

void OrderedList::insert(const ItemPtr &item)
{
    // item already exists
    if (hasItem(item)) {
        return;
    }

    int zItemIndex{0};
    if (!m_itemList.empty()) {
        zItemIndex = m_zIndex[m_itemList.back()] + 1;
    }

    qCDebug(DRAWY_LOG) << "Inserting item with index: " << zItemIndex;
    m_zIndex[item] = zItemIndex;
    m_itemList.push_back(item);
    m_itemIterMap[item] = std::prev(m_itemList.end());
}

void OrderedList::remove(const ItemPtr &item)
{
    // item already deleted
    if (!hasItem(item)) {
        return;
    }

    qCDebug(DRAWY_LOG) << "Erasing item from list";
    m_itemList.erase(m_itemIterMap[item]);
    m_itemIterMap.erase(item);
    m_zIndex.erase(item);
}

void OrderedList::bringForward(const ItemPtr &item)
{
    if (!hasItem(item)) {
        qCWarning(DRAWY_LOG) << "Item was not found in the iterator map";
        return;
    }

    auto iterator = m_itemIterMap[item];

    // if this is the last element, no need to bring it to the front
    if (iterator == std::prev(m_itemList.end())) {
        return;
    }

    // swap with next
    auto nextIterator = std::next(iterator);
    m_itemList.splice(iterator, m_itemList, nextIterator);

    const int val = m_zIndex[item];
    m_zIndex[item] = m_zIndex[*nextIterator];
    m_zIndex[*nextIterator] = val;
}

void OrderedList::sendBackward(const ItemPtr &item)
{
    if (!hasItem(item)) {
        qCWarning(DRAWY_LOG) << "Item was not found in the iterator map";
        return;
    }

    auto iterator = m_itemIterMap[item];

    // if this is the first element, no need to send it to the back
    if (iterator == m_itemList.begin()) {
        return;
    }

    // swap with previous
    auto prevIterator = std::prev(iterator);
    m_itemList.splice(prevIterator, m_itemList, iterator);

    const int val = m_zIndex[item];
    m_zIndex[item] = m_zIndex[*prevIterator];
    m_zIndex[*prevIterator] = val;
}

void OrderedList::sendToBack(const ItemPtr &item)
{
    if (!hasItem(item)) {
        qCWarning(DRAWY_LOG) << "Item was not found in the iterator map";
        return;
    }

    auto iterator = m_itemIterMap[item];
    if (iterator == m_itemList.begin()) {
        return;
    }

    auto firstIterator = m_itemList.begin();
    m_itemList.splice(firstIterator, m_itemList, iterator);
    m_zIndex[item] = m_zIndex[*firstIterator] - 1;
}

void OrderedList::bringToFront(const ItemPtr &item)
{
    if (!hasItem(item)) {
        qCWarning(DRAWY_LOG) << "Item was not found in the iterator map";
        return;
    }

    auto iterator = m_itemIterMap[item];
    if (iterator == std::prev(m_itemList.end())) {
        return;
    }

    auto lastIterator = std::prev(m_itemList.end());
    m_itemList.splice(m_itemList.end(), m_itemList, iterator);
    m_zIndex[item] = m_zIndex[*lastIterator] + 1;
}

int OrderedList::zIndex(const ItemPtr &item) const
{
    if (!hasItem(item)) {
        qCWarning(DRAWY_LOG) << "Item was not found in the iterator map";
        return -1;
    }
    return m_zIndex.at(item);
}

std::list<OrderedList::ItemPtr> OrderedList::itemList() const
{
    return m_itemList;
}
