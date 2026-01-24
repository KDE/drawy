/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "loadjobutil.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"

void LoadJobUtil::loadFile(const LoadJob::LoadInfo &info)
{
    ApplicationContext *context{ApplicationContext::instance()};
    context->reset();

    QuadTree &quadtree{context->spatialContext()->quadtree()};
    for (const auto &item : info.items) {
        quadtree.insertItem(item);
    }

    context->renderingContext()->setZoomFactor(info.zoomFactor);

    context->spatialContext()->setOffsetPos(info.offsetPos);
    context->renderingContext()->cacheGrid().markAllDirty();
    context->renderingContext()->markForRender();
    context->renderingContext()->markForUpdate();
}
