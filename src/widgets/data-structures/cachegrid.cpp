// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "cachegrid.hpp"

#include "drawy_debug.h"
#include <QPainter>

int CacheCell::counter = 0;

CacheCell::CacheCell(const QPoint &point)
    : m_point{point}
    , m_pixmap(std::make_unique<QPixmap>(CacheCell::cellSize()))
    , m_dirty(true)
{
    m_pixmap->fill(Qt::transparent);

    CacheCell::counter++;
}

CacheCell::~CacheCell()
{
    CacheCell::counter--;
}

const QPoint &CacheCell::point() const
{
    return m_point;
}

bool CacheCell::dirty() const
{
    return m_dirty;
}

QPixmap &CacheCell::pixmap() const
{
    return *m_pixmap;
}

void CacheCell::paint(const std::function<void(QPainter &)> &paintFunc)
{
    QPainter painter{m_pixmap.get()};
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setClipRegion(m_pixmap->rect());

    paintFunc(painter);
}

QRect CacheCell::rect() const
{
    const int cellW{CacheCell::cellSize().width()}, cellH{CacheCell::cellSize().height()};
    const QPoint cellPos{point().x() * cellW, point().y() * cellH};
    return {cellPos.x(), cellPos.y(), cellW, cellH};
}

void CacheCell::setDirty(bool dirty)
{
    m_dirty = dirty;
}

QSize CacheCell::cellSize()
{
    return {500, 500};
}

CacheGrid::CacheGrid(int maxSize)
{
    m_headCell->nextCell = m_tailCell;
    m_tailCell->prevCell = m_headCell;

    if (maxSize == 0) {
        throw std::logic_error("maxSize can not be zero for the cache grid");
    }
    m_maxSize = maxSize;
}

CacheGrid::~CacheGrid()
{
    qCDebug(DRAWY_LOG) << "Object deleted: CacheGrid";
}

QList<std::shared_ptr<CacheCell>> CacheGrid::queryCells(const QRect &rect)
{
    const QPoint topLeft{rect.topLeft()}, bottomRight{rect.bottomRight()};

    auto floorDivide = [](int first, int second) -> int {
        int result{first / second};
        int remainder{first % second};

        if (remainder != 0 && (first < 0) != (second < 0)) {
            result--;
        }

        return result;
    };

    int cellMinX{floorDivide(topLeft.x(), CacheCell::cellSize().width())};
    int cellMinY{floorDivide(topLeft.y(), CacheCell::cellSize().height())};
    int cellMaxX{floorDivide(bottomRight.x(), CacheCell::cellSize().width())};
    int cellMaxY{floorDivide(bottomRight.y(), CacheCell::cellSize().height())};

    int totalRows{cellMaxX - cellMinX + 1};
    int totalCols{cellMaxY - cellMinY + 1};
    long long totalCells{1LL * totalRows * totalCols};

    int row = cellMinX;
    int col = cellMinY;

    if (totalCells > m_maxSize) {
        row = cellMinX + totalRows - floorDivide(m_maxSize - 1, totalCols) - 1;
        col = cellMinY + totalCols - (m_maxSize - 1) % totalCols - 1;
    }

    QList<std::shared_ptr<CacheCell>> out{};

    while (row <= cellMaxX) {
        while (col <= cellMaxY) {
            out.push_back(cell(QPoint{row, col}));
            col++;
        }
        row++;
        col = cellMinY;
    }

    return out;
}

void CacheGrid::markDirty(const QRect &rect)
{
    QList<std::shared_ptr<CacheCell>> dirtyCells{queryCells(rect)};
    for (const std::shared_ptr<CacheCell> &cell : dirtyCells) {
        cell->setDirty(true);
    }
}

std::shared_ptr<CacheCell> CacheGrid::cell(const QPoint &point)
{
    std::shared_ptr<CacheCell> cur{};
    if (!m_grid.contains(point) || !m_grid[point]) {
        if (m_curSize == m_maxSize) {
            // deleting least recently used cell
            std::shared_ptr<CacheCell> temp{m_headCell->nextCell};
            m_headCell->nextCell = temp->nextCell;
            if (auto next = temp->nextCell.lock()) {
                next->prevCell = m_headCell;
            }
            m_grid[temp->point()] = nullptr;
            m_grid.remove(temp->point());
            temp.reset();
            m_curSize--;
        }

        cur = std::make_shared<CacheCell>(point);
        m_grid[point] = cur;
        m_curSize++;
    } else {
        cur = m_grid[point];
        if (auto prev = cur->prevCell.lock()) {
            prev->nextCell = cur->nextCell;
        }
        if (auto next = cur->nextCell.lock()) {
            next->prevCell = cur->prevCell;
        }
    }
    cur->prevCell = m_tailCell->prevCell;
    cur->nextCell = m_tailCell;
    if (auto prev = m_tailCell->prevCell.lock()) {
        prev->nextCell = cur;
    }
    m_tailCell->prevCell = cur;

    return cur;
}

int CacheGrid::size() const
{
    return m_curSize;
}

void CacheGrid::setSize(int newSize)
{
    m_maxSize = newSize;
}

void CacheGrid::markAllDirty()
{
    for (const auto &cell : std::move(m_grid)) {
        cell->setDirty(true);
    }
}

void CacheGrid::clear()
{
    m_headCell->nextCell = m_tailCell;
    m_tailCell->prevCell = m_headCell;
    m_grid.clear();
    m_curSize = 0;
}
