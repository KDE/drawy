// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdlib>
#include <memory>
#include <unordered_map>

#include "common/utils/math.hpp"
#include "item/item.hpp"
#include "orderedlist.hpp"

template<typename Shape>
QVector<std::shared_ptr<Item>> QuadTree::queryItems(const Shape &shape) const
{
    return queryItems(shape, [](const std::shared_ptr<Item> &item, const Shape &shape) {
        return item->intersects(shape);
    });
}

template<typename Shape, typename QueryCondition>
QVector<std::shared_ptr<Item>> QuadTree::queryItems(const Shape &shape, QueryCondition condition) const
{
    QVector<std::shared_ptr<Item>> curItems{};
    std::unordered_map<std::shared_ptr<Item>, bool> itemAlreadyPushed{};

    // look for matches and store the result in curItems
    query(shape, condition, curItems, itemAlreadyPushed);

    // sort based on z-index
    std::sort(curItems.begin(), curItems.end(), [&](auto &firstItem, auto &secondItem) {
        return m_orderedList->zIndex(firstItem) < m_orderedList->zIndex(secondItem);
    });

    return curItems;
};

template<typename Shape, typename QueryCondition>
void QuadTree::query(const Shape &shape,
                     QueryCondition condition,
                     QVector<std::shared_ptr<Item>> &out,
                     std::unordered_map<std::shared_ptr<Item>, bool> &itemAlreadyPushed) const
{
    if (!Common::Utils::Math::intersects(m_boundingBox, shape)) {
        return;
    }

    for (const std::shared_ptr<Item> &item : m_items) {
        if (Common::Utils::Math::intersects(item->boundingBox(), shape)) {
            if (condition(item, shape)) {
                // using the hash map because multiple nodes may have a pointer to the
                // same item
                if (!itemAlreadyPushed[item]) {
                    out.push_back(item);
                    itemAlreadyPushed[item] = true;
                }
            }
        }
    }

    // if this node has sub-regions
    if (m_topLeft != nullptr) {
        m_topLeft->query(shape, condition, out, itemAlreadyPushed);
        m_topRight->query(shape, condition, out, itemAlreadyPushed);
        m_bottomRight->query(shape, condition, out, itemAlreadyPushed);
        m_bottomLeft->query(shape, condition, out, itemAlreadyPushed);
    }
}
