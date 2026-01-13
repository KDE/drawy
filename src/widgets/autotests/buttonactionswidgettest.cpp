/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "buttonactionswidgettest.hpp"
#include "properties/widgets/buttonactionswidget.hpp"
#include <QTest>
QTEST_MAIN(ButtonActionsWidgetTest)
using namespace Qt::Literals::StringLiterals;
ButtonActionsWidgetTest::ButtonActionsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ButtonActionsWidgetTest::shouldHaveDefaultValues()
{
    const ButtonActionsWidget w;
    QCOMPARE(w.property("class"), u"drawyPropertyBarActionButton"_s);
}

#include "moc_buttonactionswidgettest.cpp"
