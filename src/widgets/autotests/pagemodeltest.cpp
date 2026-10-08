/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "pagemodeltest.hpp"
#include "page/page.hpp"
#include "page/pagemanager.hpp"
#include "page/pagemodel.hpp"
#include <QAbstractItemModelTester>
#include <QTest>
QTEST_GUILESS_MAIN(PageModelTest)

namespace
{
Page *createPage(const QString &name)
{
    auto page = new Page(nullptr);
    page->setName(name);
    return page;
}

QStringList pageNames(const PageManager &m)
{
    QStringList names;
    const auto pages = m.pages();
    for (const Page *page : pages) {
        names.append(page->name());
    }
    return names;
}
}

PageModelTest::PageModelTest(QObject *parent)
    : QObject{parent}
{
}

void PageModelTest::shouldHaveDefaultValues()
{
    PageManager m;
    const PageModel model(&m);
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(model.supportedDropActions(), Qt::MoveAction);
}

void PageModelTest::shouldFollowPageManager()
{
    PageManager m;
    PageModel model(&m);
    QAbstractItemModelTester tester(&model);
    m.insertPage(0, createPage(QStringLiteral("1")));
    m.insertPage(1, createPage(QString()));
    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(model.index(0).data().toString(), QStringLiteral("1"));
    QVERIFY(model.index(1).data().toString().isEmpty());
    QVERIFY(model.index(1).data(Qt::EditRole).toString().isEmpty());

    QVERIFY(model.index(0).data(PageModel::IsCurrentPageRole).toBool());
    m.setCurrentPage(1);
    QVERIFY(!model.index(0).data(PageModel::IsCurrentPageRole).toBool());
    QVERIFY(model.index(1).data(PageModel::IsCurrentPageRole).toBool());

    m.removePage(0);
    QCOMPARE(model.rowCount(), 1);
}

void PageModelTest::shouldRenamePage()
{
    PageManager m;
    PageModel model(&m);
    m.insertPage(0, createPage(QStringLiteral("1")));
    QVERIFY(model.setData(model.index(0), QStringLiteral(" foo ")));
    QCOMPARE(m.currentName(), QStringLiteral("foo"));
    // Empty name is refused
    QVERIFY(!model.setData(model.index(0), QStringLiteral("  ")));
    QCOMPARE(m.currentName(), QStringLiteral("foo"));
}

void PageModelTest::shouldMoveRows()
{
    PageManager m;
    PageModel model(&m);
    QAbstractItemModelTester tester(&model);
    m.insertPage(0, createPage(QStringLiteral("1")));
    m.insertPage(1, createPage(QStringLiteral("2")));
    m.insertPage(2, createPage(QStringLiteral("3")));

    // Move down: destination is the row before which the page is inserted
    QVERIFY(model.moveRow({}, 0, {}, 3));
    QCOMPARE(pageNames(m), (QStringList{QStringLiteral("2"), QStringLiteral("3"), QStringLiteral("1")}));
    QCOMPARE(m.currentPage(), 2);
    QVERIFY(model.index(2).data(PageModel::IsCurrentPageRole).toBool());

    // Move up
    QVERIFY(model.moveRow({}, 2, {}, 0));
    QCOMPARE(pageNames(m), (QStringList{QStringLiteral("1"), QStringLiteral("2"), QStringLiteral("3")}));
    QCOMPARE(m.currentPage(), 0);

    // Move onto itself
    QVERIFY(!model.moveRow({}, 1, {}, 1));
    QVERIFY(!model.moveRow({}, 1, {}, 2));
    QVERIFY(!model.moveRow({}, 1, {}, 4));
}

#include "moc_pagemodeltest.cpp"
