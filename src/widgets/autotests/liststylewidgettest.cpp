/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "liststylewidgettest.hpp"
#include "properties/widgets/liststylewidget.hpp"
#include <QComboBox>
#include <QHBoxLayout>
#include <QTest>

using namespace Qt::Literals::StringLiterals;

QTEST_MAIN(ListStyleWidgetTest)

ListStyleWidgetTest::ListStyleWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ListStyleWidgetTest::shouldHaveDefaultValues()
{
    const auto parent = new QWidget();
    ListStyleWidget w(parent);

    QVERIFY(!w.name().isEmpty());
    QVERIFY(w.widget());
    QCOMPARE(w.widget()->objectName(), u"m_widget"_s);

    auto m_widget = parent->findChild<QWidget *>(u"m_widget"_s);
    QVERIFY(m_widget);

    auto layout = m_widget->findChild<QHBoxLayout *>(u"layout"_s);
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), {});

    auto comboBox = m_widget->findChild<QComboBox *>(u"m_comboBox"_s);
    QVERIFY(comboBox);
    QCOMPARE(comboBox->count(), 9);
    QCOMPARE(comboBox->currentData().toString(), u"None"_s);
}

#include "moc_liststylewidgettest.cpp"
