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
#include "page/page.hpp"
#include "page/pagemanager.hpp"
#include <functional>

void LoadJobUtil::loadFile(ApplicationContext *context, const LoadJobUtil::DeserializeInfo &info)
{
    std::function<void(const std::shared_ptr<Item> &, QuadTree &quadtree)> processItem = [&](const std::shared_ptr<Item> &item, QuadTree &quadtree) {
        if (item->formType() == Item::FormType::Group) {
            auto groupItem = std::static_pointer_cast<GroupItem>(item);

            auto children = groupItem->unGroup();
            for (const auto &child : std::as_const(children)) {
                processItem(child, quadtree);
            }

            for (const auto &child : std::as_const(children)) {
                quadtree.deleteItem(child, false);
            }

            groupItem->setTransform({});
            groupItem->group(children);
        }
        quadtree.insertItem(item);
    };

    context->reset();
    QList<Page *> pages;
    pages.reserve(info.pages.count());
    for (const auto &p : info.pages) {
        auto page = new Page(context);
        page->setName(p.name.isEmpty() ? Page::defaultName(pages.count()) : p.name);
        page->setOffsetPos(p.offsetPos);
        page->setZoomFactor(p.zoomFactor);
        QuadTree &quadtree{page->quadtree()};
        for (const auto &item : p.items) {
            processItem(item, quadtree);
        }
        pages.append(page);
    }
    if (pages.isEmpty()) {
        auto page = new Page(context);
        page->setName(Page::defaultName(0));
        pages.append(page);
    }

    PageManager *pageManager = context->pageManager();
    pageManager->setPages(pages);
    if (info.currentPage > 0 && info.currentPage < pages.count()) {
        pageManager->setCurrentPage(info.currentPage);
    }

    context->renderingContext()->setZoomFactor(pageManager->currentPageObject()->zoomFactor());
    context->renderingContext()->cacheGrid().markAllDirty();
    context->renderingContext()->markForRender();
    context->renderingContext()->markForUpdate();
}
