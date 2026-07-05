/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "cornerrectangletypewidgettest.hpp"
#include "item/itemutils.hpp"
#include "properties/widgets/buttonactionswidget.hpp"
#include "properties/widgets/cornerrectangletypewidget.hpp"
#include <QHBoxLayout>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(CornerRectangleTypeWidgetTest)
CornerRectangleTypeWidgetTest::CornerRectangleTypeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CornerRectangleTypeWidgetTest::shouldHaveDefaultValues()
{
    auto parent = new QWidget();
    const CornerRectangleTypeWidget w(parent);
    QVERIFY(!w.name().isEmpty());
    QVERIFY(w.widget());
    QCOMPARE(w.widget()->objectName(), u"m_widget"_s);

    QVERIFY(!w.name().isEmpty());

    auto m_widget = parent->findChild<QWidget *>(u"m_widget"_s);
    QVERIFY(m_widget);

    auto layout = m_widget->findChild<QHBoxLayout *>(u"layout"_s);
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins{});

    auto roundedCornerButton = m_widget->findChild<ButtonActionsWidget *>(u"roundedCornerButton"_s);
    QVERIFY(roundedCornerButton);
    QCOMPARE(roundedCornerButton->property("corner-rectangle-type"), ItemUtils::convertItemCornerRectangleTypeEnumToString(Item::CornerRectangleType::Rounded));
    QVERIFY(!roundedCornerButton->toolTip().isEmpty());
    QVERIFY(roundedCornerButton->isCheckable());

    auto pointedCornerButton = m_widget->findChild<ButtonActionsWidget *>(u"pointedCornerButton"_s);
    QVERIFY(pointedCornerButton);
    QCOMPARE(pointedCornerButton->property("corner-rectangle-type"), ItemUtils::convertItemCornerRectangleTypeEnumToString(Item::CornerRectangleType::Pointed));
    QVERIFY(!pointedCornerButton->toolTip().isEmpty());
    QVERIFY(pointedCornerButton->isCheckable());
}

#include "moc_cornerrectangletypewidgettest.cpp"
