/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "pagemanagertest.hpp"
#include "page/page.hpp"
#include "page/pagemanager.hpp"
#include <QPointer>
#include <QSignalSpy>
#include <QTest>
QTEST_GUILESS_MAIN(PageManagerTest)

namespace
{
Page *createPage(const QString &name)
{
    auto page = new Page(nullptr);
    page->setName(name);
    return page;
}
}

PageManagerTest::PageManagerTest(QObject *parent)
    : QObject{parent}
{
}

void PageManagerTest::shouldHaveDefaultValues()
{
    const PageManager m;
    QVERIFY(m.pages().isEmpty());
    QCOMPARE(m.currentPage(), -1);
    QVERIFY(m.currentName().isEmpty());
}

void PageManagerTest::shouldInsertPages()
{
    PageManager m;
    QSignalSpy spy(&m, &PageManager::currentPageChanged);
    auto page1 = createPage(QStringLiteral("1"));
    m.insertPage(0, page1);
    QCOMPARE(m.pages().count(), 1);
    QCOMPARE(m.currentPage(), 0);
    QCOMPARE(m.currentName(), QStringLiteral("1"));
    QCOMPARE(page1->parent(), &m);
    QCOMPARE(spy.count(), 1);

    // Append at the end
    m.insertPage(1, createPage(QStringLiteral("2")));
    QCOMPARE(m.pages().count(), 2);
    QCOMPARE(m.pages().at(1)->name(), QStringLiteral("2"));
    QCOMPARE(m.currentPage(), 0);
    QCOMPARE(spy.count(), 1);
}

void PageManagerTest::shouldNotInsertInvalidIndex()
{
    PageManager m;
    auto page = createPage(QStringLiteral("1"));
    m.insertPage(1, page);
    m.insertPage(-1, page);
    QVERIFY(m.pages().isEmpty());
    QCOMPARE(m.currentPage(), -1);
    m.insertPage(0, nullptr);
    QVERIFY(m.pages().isEmpty());
    delete page;
}

void PageManagerTest::shouldRemoveCurrentPage()
{
    PageManager m;
    m.insertPage(0, createPage(QStringLiteral("1")));
    m.insertPage(1, createPage(QStringLiteral("2")));
    QPointer<Page> page = m.pages().at(0);
    QSignalSpy spy(&m, &PageManager::currentPageChanged);
    m.removePage(0);
    QVERIFY(page.isNull());
    QCOMPARE(m.currentPage(), 0);
    QCOMPARE(m.currentName(), QStringLiteral("2"));
    QCOMPARE(spy.count(), 1);
}

void PageManagerTest::shouldShiftCurrentPageOnRemove()
{
    PageManager m;
    m.insertPage(0, createPage(QStringLiteral("1")));
    m.insertPage(1, createPage(QStringLiteral("2")));
    m.insertPage(2, createPage(QStringLiteral("3")));
    m.setCurrentPage(2);
    m.removePage(0);
    QCOMPARE(m.currentPage(), 1);
    QCOMPARE(m.currentName(), QStringLiteral("3"));

    // Remove current page which is the last one
    m.removePage(1);
    QCOMPARE(m.currentPage(), 0);
    QCOMPARE(m.currentName(), QStringLiteral("2"));
}

void PageManagerTest::shouldShiftCurrentPageOnInsert()
{
    PageManager m;
    m.insertPage(0, createPage(QStringLiteral("1")));
    m.insertPage(0, createPage(QStringLiteral("0")));
    QCOMPARE(m.currentPage(), 1);
    QCOMPARE(m.currentName(), QStringLiteral("1"));
}

void PageManagerTest::shouldRemoveLastPage()
{
    PageManager m;
    m.insertPage(0, createPage(QStringLiteral("1")));
    m.removePage(0);
    QVERIFY(m.pages().isEmpty());
    QCOMPARE(m.currentPage(), -1);
    QVERIFY(m.currentName().isEmpty());

    m.removePage(0);
    QCOMPARE(m.currentPage(), -1);
}

void PageManagerTest::shouldNotSetInvalidCurrentPage()
{
    PageManager m;
    m.setCurrentPage(0);
    QCOMPARE(m.currentPage(), -1);
    m.insertPage(0, createPage(QStringLiteral("1")));
    m.setCurrentPage(1);
    QCOMPARE(m.currentPage(), 0);

    QSignalSpy spy(&m, &PageManager::currentPageChanged);
    m.setCurrentPage(0);
    QCOMPARE(spy.count(), 0);
}

void PageManagerTest::shouldRenamePage()
{
    PageManager m;
    m.insertPage(0, createPage(QStringLiteral("1")));
    m.renamePage(0, QStringLiteral("foo"));
    QCOMPARE(m.currentName(), QStringLiteral("foo"));
    m.renamePage(1, QStringLiteral("bla"));
    QCOMPARE(m.currentName(), QStringLiteral("foo"));
}

void PageManagerTest::shouldSetPages()
{
    PageManager m;
    m.insertPage(0, createPage(QStringLiteral("1")));
    m.insertPage(1, createPage(QStringLiteral("2")));
    QPointer<Page> oldPage = m.pages().at(0);
    QPointer<Page> keptPage = m.pages().at(1);
    m.setCurrentPage(1);

    QSignalSpy spy(&m, &PageManager::currentPageChanged);
    auto newPage = createPage(QStringLiteral("3"));
    m.setPages({keptPage.data(), newPage});
    QVERIFY(oldPage.isNull());
    QVERIFY(!keptPage.isNull());
    QCOMPARE(newPage->parent(), &m);
    QCOMPARE(m.currentPage(), 0);
    QCOMPARE(m.currentName(), QStringLiteral("2"));
    QCOMPARE(spy.count(), 1);

    m.setPages({});
    QVERIFY(keptPage.isNull());
    QCOMPARE(m.currentPage(), -1);
}

void PageManagerTest::shouldMovePage()
{
    PageManager m;
    m.insertPage(0, createPage(QStringLiteral("1")));
    m.insertPage(1, createPage(QStringLiteral("2")));
    m.insertPage(2, createPage(QStringLiteral("3")));
    QSignalSpy movedSpy(&m, &PageManager::pageMoved);

    // Current page follows the moved page
    m.movePage(0, 2);
    QCOMPARE(movedSpy.count(), 1);
    QCOMPARE(m.currentPage(), 2);
    QCOMPARE(m.currentName(), QStringLiteral("1"));
    QCOMPARE(m.pages().at(0)->name(), QStringLiteral("2"));

    // Moving another page over the current one shifts it
    m.movePage(0, 2);
    QCOMPARE(m.currentPage(), 1);
    QCOMPARE(m.currentName(), QStringLiteral("1"));

    // Invalid index
    m.movePage(0, 3);
    QCOMPARE(movedSpy.count(), 2);
}

#include "moc_pagemanagertest.cpp"
