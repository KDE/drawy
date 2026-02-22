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
#include "item/group.hpp"
#include <functional>

void LoadJobUtil::loadFile(ApplicationContext *context, const LoadJob::LoadInfo &info)
{
    context->reset();

    QuadTree &quadtree{context->spatialContext()->quadtree()};

    std::function<void(const std::shared_ptr<Item> &)> processItem = [&](const std::shared_ptr<Item> &item) {
        if (item->formType() == Item::FormType::Group) {
            auto groupItem = std::static_pointer_cast<GroupItem>(item);

            auto children = groupItem->unGroup();
            for (const auto &child : children) {
                processItem(child);
            }

            for (const auto &child : children) {
                quadtree.deleteItem(child, false);
            }

            groupItem->setTransform({});
            groupItem->group(children);
        }
        quadtree.insertItem(item);
    };

    for (const auto &item : info.items) {
        processItem(item);
    }

    context->renderingContext()->setZoomFactor(info.zoomFactor);

    context->spatialContext()->setOffsetPos(info.offsetPos);
    context->renderingContext()->cacheGrid().markAllDirty();
    context->renderingContext()->markForRender();
    context->renderingContext()->markForUpdate();
}
