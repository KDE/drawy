// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <list>
#include <memory>
#include <unordered_map>
class Item;

// Keeps track of the z-index of every item
class OrderedList
{
public:
    using ItemPtr = std::shared_ptr<Item>;

private:
    std::unordered_map<ItemPtr, std::list<ItemPtr>::iterator> m_itemIterMap;
    std::list<ItemPtr> m_itemList;
    std::unordered_map<ItemPtr, int> m_zIndex;

public:
    ~OrderedList();

    void insert(const ItemPtr &item);
    void remove(const ItemPtr &item);

    void bringForward(const ItemPtr &item);
    void sendBackward(const ItemPtr &item);
    void sendToBack(const ItemPtr &item);
    void bringToFront(const ItemPtr &item);
    bool hasItem(const ItemPtr &item) const;

    [[nodiscard]] int zIndex(const ItemPtr &item) const;
};
