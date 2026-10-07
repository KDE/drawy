// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "spatialcontext.hpp"

#include <memory>

#include "applicationcontext.hpp"
#include "command/commandhistory.hpp"
#include "coordinatetransformer.hpp"
#include "data-structures/quadtree.hpp"
#include "drawy_debug.h"
#include "page/page.hpp"
#include "page/pagemanager.hpp"

SpatialContext::SpatialContext(ApplicationContext *context)
    : QObject{context}
    , m_applicationContext{context}
{
    m_coordinateTransformer = std::make_unique<CoordinateTransformer>(m_applicationContext);
}

SpatialContext::~SpatialContext()
{
    qCDebug(DRAWY_LOG) << "Object deleted: SpatialContext";
}

Page *SpatialContext::currentPage() const
{
    Page *page{m_applicationContext->pageManager()->currentPageObject()};
    Q_ASSERT(page);
    return page;
}

QuadTree &SpatialContext::quadtree() const
{
    return currentPage()->quadtree();
}

CoordinateTransformer &SpatialContext::coordinateTransformer() const
{
    return *m_coordinateTransformer;
}

CommandHistory *SpatialContext::commandHistory() const
{
    return currentPage()->commandHistory();
}

const QPointF &SpatialContext::offsetPos() const
{
    return currentPage()->offsetPos();
}

void SpatialContext::setOffsetPos(const QPointF &pos)
{
    currentPage()->setOffsetPos(pos);
}

void SpatialContext::reset()
{
    quadtree().clear();
    commandHistory()->clear();
    setOffsetPos(QPointF{0, 0});
}

#include "moc_spatialcontext.cpp"
