/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "alignmentwidgettest.hpp"
#include "properties/widgets/alignmentwidget.hpp"
#include "properties/widgets/buttonactionswidget.hpp"
#include <QGridLayout>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(AlignmentWidgetTest)
AlignmentWidgetTest::AlignmentWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AlignmentWidgetTest::shouldHaveDefaultValues()
{
    auto parent = new QWidget();
    const AlignmentWidget w(nullptr, parent);
    QVERIFY(!w.name().isEmpty());
    auto m_widget = parent->findChild<QWidget *>(u"m_widget"_s);
    QVERIFY(m_widget);

    auto layout = parent->findChild<QGridLayout *>(u"layout"_s);
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins{});

    auto alignLeftButton = m_widget->findChild<ButtonActionsWidget *>(u"alignLeftButton"_s);
    QVERIFY(alignLeftButton);

    auto centralHorizontalButton = m_widget->findChild<ButtonActionsWidget *>(u"centralHorizontalButton"_s);
    QVERIFY(centralHorizontalButton);

    auto alignRightButton = m_widget->findChild<ButtonActionsWidget *>(u"alignRightButton"_s);
    QVERIFY(alignRightButton);

    auto alignTopButton = m_widget->findChild<ButtonActionsWidget *>(u"alignTopButton"_s);
    QVERIFY(alignTopButton);

    auto centralVerticalButton = m_widget->findChild<ButtonActionsWidget *>(u"centralVerticalButton"_s);
    QVERIFY(centralVerticalButton);

    auto alignBottomButton = m_widget->findChild<ButtonActionsWidget *>(u"alignBottomButton"_s);
    QVERIFY(alignBottomButton);
}

#include "moc_alignmentwidgettest.cpp"
