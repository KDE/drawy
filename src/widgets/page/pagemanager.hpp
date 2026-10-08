/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_private_export.h"
#include <QList>
#include <QObject>
class Page;
class LIBDRAWYWIDGETS_TESTS_EXPORT PageManager : public QObject
{
    Q_OBJECT
public:
    explicit PageManager(QObject *parent = nullptr);
    ~PageManager() override;

    void renamePage(int index, const QString &name);

    [[nodiscard]] QString currentName() const;

    [[nodiscard]] int currentPage() const;
    [[nodiscard]] Page *currentPageObject() const;
    void setCurrentPage(int newCurrentPage);

    // PageManager takes ownership of page
    void insertPage(int index, Page *page);

    void removePage(int index);

    [[nodiscard]] QList<Page *> pages() const;
    // PageManager takes ownership of newPages and deletes the previous pages
    void setPages(const QList<Page *> &newPages);

    void movePage(int from, int to);

Q_SIGNALS:
    void currentPageChanged(int index);
    void pageInserted(int index);
    void pageRemoved(int index);
    void pageMoved(int from, int to);
    void pageRenamed(int index, const QString &name);
    void pagesReset();

private:
    [[nodiscard]] bool isIndexValid(int index) const;
    void updateCurrentPage(int index);
    int mCurrentPage = -1;
    QList<Page *> mPages;
};
