/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "stokestylewidgettest.hpp"
#include "properties/widgets/buttonactionswidget.hpp"
#include "properties/widgets/stokestylewidget.hpp"
#include <QHBoxLayout>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(StokeStyleWidgetTest)

StokeStyleWidgetTest::StokeStyleWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void StokeStyleWidgetTest::shouldHaveDefaultValues()
{
    auto parent = new QWidget();
    const StokeStyleWidget w(parent);
    auto m_widget = parent->findChild<QWidget *>(u"m_widget"_s);
    QVERIFY(m_widget);

    auto layout = m_widget->findChild<QHBoxLayout *>(u"layout"_s);
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins{});

    auto solidButton = m_widget->findChild<ButtonActionsWidget *>(u"solidButton"_s);
    QVERIFY(solidButton);
    QCOMPARE(solidButton->property("stroke-style"), u"Solid"_s);
    QVERIFY(!solidButton->toolTip().isEmpty());
    QVERIFY(solidButton->isCheckable());

    auto dashLineButton = m_widget->findChild<ButtonActionsWidget *>(u"dashLineButton"_s);
    QVERIFY(dashLineButton);
    QCOMPARE(dashLineButton->property("stroke-style"), u"DashLine"_s);
    QVERIFY(!dashLineButton->toolTip().isEmpty());
    QVERIFY(dashLineButton->isCheckable());

    auto dotLineButton = m_widget->findChild<ButtonActionsWidget *>(u"dotLineButton"_s);
    QVERIFY(dotLineButton);
    QCOMPARE(dotLineButton->property("stroke-style"), u"DotLine"_s);
    QVERIFY(!dotLineButton->toolTip().isEmpty());
    QVERIFY(dotLineButton->isCheckable());
}

#include "moc_stokestylewidgettest.cpp"
