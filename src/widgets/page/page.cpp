/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "page.hpp"
#include "canvas/canvas.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"

Page::Page(ApplicationContext *context)
    : QObject(context)
    , m_applicationContext(context)
{
    if (m_applicationContext) {
        auto canvas{m_applicationContext->renderingContext()->canvas()};
        m_quadtree = std::make_unique<QuadTree>(QRect{{0, 0}, canvas->sizeHint()}, 10000);
    }
}

Page::~Page() = default;

QString Page::name() const
{
    return mName;
}

void Page::setName(const QString &newName)
{
    mName = newName;
}
