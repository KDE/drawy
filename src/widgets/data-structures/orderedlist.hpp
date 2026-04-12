// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "libdrawywidgets_private_export.h"
#include <list>
#include <memory>
#include <unordered_map>
class Item;

template<typename ItemPointer>
struct ItemOrderPosition {
    ItemPointer previousItem{};
    ItemPointer nextItem{};
};

// Keeps track of the z-index of every item
class LIBDRAWYWIDGETS_TESTS_EXPORT OrderedList
{
public:
    using ItemPtr = std::shared_ptr<Item>;
    using Position = ItemOrderPosition<ItemPtr>;

public:
    ~OrderedList();

    void clear();

    void insert(const ItemPtr &item);
    void remove(const ItemPtr &item);

    void bringForward(const ItemPtr &item);
    void sendBackward(const ItemPtr &item);
    void sendToBack(const ItemPtr &item);
    void bringToFront(const ItemPtr &item);

    [[nodiscard]] bool hasItem(const ItemPtr &item) const;

    [[nodiscard]] int zIndex(const ItemPtr &item) const;
    [[nodiscard]] Position position(const ItemPtr &item) const;
    void restorePosition(const ItemPtr &item, const Position &position);

    [[nodiscard]] std::list<ItemPtr> itemList() const;

    [[nodiscard]] bool canMoveBackwards(const ItemPtr &item) const;
    [[nodiscard]] bool canMoveForwards(const ItemPtr &item) const;

private:
    void normalizeZIndexes();
    void updateZIndexFromNeighbors(const ItemPtr &item);

    std::unordered_map<ItemPtr, std::list<ItemPtr>::iterator> m_itemIterMap;
    std::list<ItemPtr> m_itemList;
    std::unordered_map<ItemPtr, int> m_zIndex;
};
