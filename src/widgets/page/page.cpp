/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "page.hpp"
#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"

Page::Page(ApplicationContext *context)
    : QObject(context)
    , m_applicationContext(context)
{
    if (m_applicationContext) {
        auto canvas{m_applicationContext->renderingContext()->canvas()};
        m_quadtree = std::make_unique<QuadTree>(QRect{{0, 0}, canvas->sizeHint()}, 10000);
        m_commandHistory = std::make_unique<CommandHistory>(m_applicationContext);
    }
}

Page::~Page() = default;

QuadTree &Page::quadtree() const
{
    return *m_quadtree;
}

qreal Page::zoomFactor() const
{
    return m_zoomFactor;
}

void Page::setZoomFactor(qreal newZoomFactor)
{
    m_zoomFactor = newZoomFactor;
}

QString Page::name() const
{
    return mName;
}

void Page::setName(const QString &newName)
{
    mName = newName;
}

CommandHistory *Page::commandHistory() const
{
    return m_commandHistory.get();
}

const QPointF &Page::offsetPos() const
{
    return m_offsetPos;
}

void Page::setOffsetPos(const QPointF &pos)
{
    m_offsetPos = pos;
}

#include "moc_page.cpp"
