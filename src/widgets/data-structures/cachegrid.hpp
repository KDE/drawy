// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QHash>
#include <QPixmap>
#include <QPoint>
#include <limits>

class CacheGrid;

// Based on LRU cache
class CacheCell
{
public:
    explicit CacheCell(const QPoint &point, const QSize &size);
    ~CacheCell();

    [[nodiscard]] const QSize &cellSize() const;

    [[nodiscard]] QRect rect() const;
    [[nodiscard]] const QPoint &point() const;
    [[nodiscard]] bool dirty() const;
    void setDirty(bool dirty);

    [[nodiscard]] QPixmap &pixmap() const;
    void paint(const std::function<void(QPainter &)> &paintFunc);

private:
    QPoint m_point{};
    std::unique_ptr<QPixmap> m_pixmap{nullptr};
    std::weak_ptr<CacheCell> nextCell{};
    std::weak_ptr<CacheCell> prevCell{};
    bool m_dirty{};

    QSize m_size{};

    // CacheGrid can access private members
    friend CacheGrid;
};

class CacheGrid
{
public:
    explicit CacheGrid(int maxSize, const QSize &cellSize);
    ~CacheGrid();

    [[nodiscard]] QList<std::shared_ptr<CacheCell>> queryCells(const QRect &rect);
    [[nodiscard]] std::shared_ptr<CacheCell> cell(const QPoint &point);
    void markDirty(const QRect &rect);
    void markAllDirty();

    void setSize(int newSize);
    [[nodiscard]] int size() const;

    [[nodiscard]] const QSize &cellSize() const;

    void setBounds(const QRect &rect);

    void clear();

private:
    QHash<QPoint, std::shared_ptr<CacheCell>> m_grid{};
    std::shared_ptr<CacheCell> m_headCell{std::make_shared<CacheCell>(QPoint{0, 0}, QSize{0, 0})};
    std::shared_ptr<CacheCell> m_tailCell{std::make_shared<CacheCell>(QPoint{0, 0}, QSize{0, 0})};

    QSize m_cellSize{};
    int m_curSize{0};
    int m_maxSize{0};

    int m_cellMinBoundX{std::numeric_limits<int>::min()};
    int m_cellMaxBoundX{std::numeric_limits<int>::max()};
    int m_cellMinBoundY{std::numeric_limits<int>::min()};
    int m_cellMaxBoundY{std::numeric_limits<int>::max()};
};
