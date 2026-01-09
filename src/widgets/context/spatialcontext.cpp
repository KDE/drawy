// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "spatialcontext.hpp"

#include <memory>

#include "applicationcontext.hpp"
#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "coordinatetransformer.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "drawy_debug.h"
#include "renderingcontext.hpp"

SpatialContext::SpatialContext(ApplicationContext *context)
    : QObject{context}
    , m_applicationContext{context}
{
}

SpatialContext::~SpatialContext()
{
    qCDebug(DRAWY_LOG) << "Object deleted: SpatialContext";
}

void SpatialContext::setSpatialContext()
{
    Canvas &canvas{m_applicationContext->renderingContext().canvas()};

    m_quadtree = std::make_unique<QuadTree>(QRect{{0, 0}, canvas.sizeHint()}, 100);
    m_coordinateTransformer = std::make_unique<CoordinateTransformer>(m_applicationContext);
    m_cacheGrid = std::make_unique<CacheGrid>(100);
    m_commandHistory = std::make_unique<CommandHistory>(m_applicationContext);
}

QuadTree &SpatialContext::quadtree() const
{
    return *m_quadtree;
}

CacheGrid &SpatialContext::cacheGrid() const
{
    return *m_cacheGrid;
}

CoordinateTransformer &SpatialContext::coordinateTransformer() const
{
    return *m_coordinateTransformer;
}

CommandHistory &SpatialContext::commandHistory() const
{
    return *m_commandHistory;
}

const QPointF &SpatialContext::offsetPos() const
{
    return m_offsetPos;
}

void SpatialContext::setOffsetPos(const QPointF &pos)
{
    m_offsetPos = pos;
}

void SpatialContext::reset()
{
    quadtree().clear();
    cacheGrid().markAllDirty();
    commandHistory().clear();
    setOffsetPos(QPointF{0, 0});
}
