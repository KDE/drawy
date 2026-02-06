/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "selectiontooltest.hpp"
#include "tools/selectiontool/selectiontool.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(SelectionToolTest)
using namespace Qt::Literals::StringLiterals;
SelectionToolTest::SelectionToolTest(QObject *parent)
    : QObject{parent}
{
}

void SelectionToolTest::shouldHaveDefaultValues()
{
    const SelectionTool t(nullptr);
    QVERIFY(!t.tooltip().isEmpty());
    QCOMPARE(t.icon(), u"tool_rect_selection"_s);
    QCOMPARE(t.type(), Tool::Type::Selection);

    QCOMPARE(t.properties().count(), 0);
}

#include "moc_selectiontooltest.cpp"
