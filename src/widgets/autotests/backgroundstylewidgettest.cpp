/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "backgroundstylewidgettest.hpp"
#include "properties/widgets/backgroundstylewidget.hpp"
#include "properties/widgets/buttonactionswidget.hpp"
#include <QGridLayout>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(BackgroundStyleWidgetTest)
BackgroundStyleWidgetTest::BackgroundStyleWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void BackgroundStyleWidgetTest::shouldHaveDefaultValues()
{
    auto parent = new QWidget();
    const BackgroundStyleWidget w(parent);
    QVERIFY(!w.name().isEmpty());

    auto m_widget = parent->findChild<QWidget *>(u"m_widget"_s);
    QVERIFY(m_widget);

    auto layout = m_widget->findChild<QHBoxLayout *>(u"layout"_s);
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins{});

    auto solidButton = m_widget->findChild<ButtonActionsWidget *>(u"solidButton"_s);
    QVERIFY(solidButton);
    QCOMPARE(solidButton->property("background-style"), u"Solid"_s);
    QVERIFY(!solidButton->toolTip().isEmpty());
    QVERIFY(solidButton->isCheckable());

    auto diagonalCrossButton = m_widget->findChild<ButtonActionsWidget *>(u"diagonalCrossButton"_s);
    QVERIFY(diagonalCrossButton);
    QCOMPARE(diagonalCrossButton->property("background-style"), u"CrossDiagonal"_s);
    QVERIFY(!diagonalCrossButton->toolTip().isEmpty());
    QVERIFY(diagonalCrossButton->isCheckable());

    auto diagonalButton = m_widget->findChild<ButtonActionsWidget *>(u"diagonalButton"_s);
    QVERIFY(diagonalButton);
    QCOMPARE(diagonalButton->property("background-style"), u"Diagonal"_s);
    QVERIFY(!diagonalButton->toolTip().isEmpty());
    QVERIFY(diagonalButton->isCheckable());
}

#include "moc_backgroundstylewidgettest.cpp"
