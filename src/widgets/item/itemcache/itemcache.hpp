// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QPainter>
#include <unordered_map>
class Item;
class CacheGrid;

class ItemCache
{
public:
    ItemCache() = default;
    ~ItemCache() = default;

    void drawCached(QPainter &painter, const std::shared_ptr<Item> &item, const QRect &queryRegion, const QPointF &offset);
    void clear();

private:
    std::unordered_map<std::shared_ptr<Item>, std::unique_ptr<CacheGrid>> m_cacheGrids;
};
