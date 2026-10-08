/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "pagemanager.hpp"
#include "drawy_page_debug.h"
#include "page.hpp"

PageManager::PageManager(QObject *parent)
    : QObject{parent}
{
}

PageManager::~PageManager() = default;

bool PageManager::isIndexValid(int index) const
{
    return index >= 0 && index < mPages.count();
}

void PageManager::movePage(int from, int to)
{
    if (from < 0 || from >= mPages.count() || to < 0 || to >= mPages.count()) {
        qCWarning(DRAWY_PAGE_LOG) << "Invalid index: to " << to << " from " << from;
        return;
    }
    const Page *current{currentPageObject()};
    mPages.move(from, to);
    Q_EMIT pageMoved(from, to);
    // Keep the same page current, its index may have changed
    if (current) {
        updateCurrentPage(mPages.indexOf(current));
    }
}

void PageManager::updateCurrentPage(int index)
{
    if (mCurrentPage != index) {
        mCurrentPage = index;
        Q_EMIT currentPageChanged(mCurrentPage);
    }
}

void PageManager::insertPage(int index, Page *page)
{
    if (!page) {
        qCWarning(DRAWY_PAGE_LOG) << "page is null";
        return;
    }
    if (index < 0 || index > mPages.count()) {
        qCWarning(DRAWY_PAGE_LOG) << "invalid index" << index;
        return;
    }
    page->setParent(this);
    mPages.insert(index, page);
    if (mCurrentPage == -1) {
        updateCurrentPage(0);
    } else if (index <= mCurrentPage) {
        updateCurrentPage(mCurrentPage + 1);
    }
    Q_EMIT pageInserted(index);
}

void PageManager::removePage(int index)
{
    if (!isIndexValid(index)) {
        qCWarning(DRAWY_PAGE_LOG) << "invalid index" << index;
        return;
    }
    delete mPages.takeAt(index);
    if (mPages.isEmpty()) {
        updateCurrentPage(-1);
    } else if (index < mCurrentPage || mCurrentPage >= mPages.count()) {
        updateCurrentPage(mCurrentPage - 1);
    } else if (index == mCurrentPage) {
        // The current page changed even if its index is the same
        Q_EMIT currentPageChanged(mCurrentPage);
    }
    Q_EMIT pageRemoved(index);
}

QList<Page *> PageManager::pages() const
{
    return mPages;
}

void PageManager::setPages(const QList<Page *> &newPages)
{
    for (Page *page : std::as_const(mPages)) {
        if (!newPages.contains(page)) {
            delete page;
        }
    }
    mPages = newPages;
    for (Page *page : std::as_const(mPages)) {
        page->setParent(this);
    }
    mCurrentPage = mPages.isEmpty() ? -1 : 0;
    Q_EMIT currentPageChanged(mCurrentPage);
    Q_EMIT pagesReset();
}

void PageManager::renamePage(int index, const QString &name)
{
    if (isIndexValid(index)) {
        mPages[index]->setName(name);
        Q_EMIT pageRenamed(index, name);
    } else {
        qCWarning(DRAWY_PAGE_LOG) << "invalid index" << index;
    }
}

QString PageManager::currentName() const
{
    if (isIndexValid(mCurrentPage)) {
        return mPages.at(mCurrentPage)->name();
    }
    return {};
}

int PageManager::currentPage() const
{
    return mCurrentPage;
}

Page *PageManager::currentPageObject() const
{
    if (isIndexValid(mCurrentPage)) {
        return mPages.at(mCurrentPage);
    }
    return nullptr;
}

void PageManager::setCurrentPage(int newCurrentPage)
{
    if (!isIndexValid(newCurrentPage)) {
        qCWarning(DRAWY_PAGE_LOG) << "invalid index" << newCurrentPage;
        return;
    }
    updateCurrentPage(newCurrentPage);
}

#include "moc_pagemanager.cpp"
