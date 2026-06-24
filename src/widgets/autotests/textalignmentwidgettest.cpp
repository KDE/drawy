/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "textalignmentwidgettest.hpp"

#include "properties/widgets/textalignmentwidget.hpp"
#include <QHBoxLayout>
#include <QTest>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;

QTEST_MAIN(TextAlignmentWidgetTest)
TextAlignmentWidgetTest::TextAlignmentWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void TextAlignmentWidgetTest::shouldHaveDefaultValues()
{
    const auto parent = new QWidget();
    const TextAlignmentWidget w(parent);
    QVERIFY(!w.name().isEmpty());

    auto m_widget = parent->findChild<QWidget *>(u"m_widget"_s);
    QVERIFY(m_widget);

    auto layout = m_widget->findChild<QHBoxLayout *>(u"layout"_s);
    QVERIFY(layout);
    QCOMPARE(layout->alignment(), Qt::AlignCenter);

    auto m_alignLeft = m_widget->findChild<QToolButton *>(u"m_alignLeft"_s);
    QVERIFY(m_alignLeft);
    QVERIFY(m_alignLeft->isCheckable());
    QVERIFY(!m_alignLeft->toolTip().isEmpty());

    auto m_alignCenter = m_widget->findChild<QToolButton *>(u"m_alignCenter"_s);
    QVERIFY(m_alignCenter);
    QVERIFY(m_alignCenter->isCheckable());
    QVERIFY(!m_alignCenter->toolTip().isEmpty());

    auto m_alignRight = m_widget->findChild<QToolButton *>(u"m_alignRight"_s);
    QVERIFY(m_alignRight);
    QVERIFY(m_alignRight->isCheckable());
    QVERIFY(!m_alignRight->toolTip().isEmpty());

    auto m_alignJustify = m_widget->findChild<QToolButton *>(u"m_alignJustify"_s);
    QVERIFY(m_alignJustify);
    QVERIFY(m_alignJustify->isCheckable());
    QVERIFY(!m_alignJustify->toolTip().isEmpty());
}

#include "moc_textalignmentwidgettest.cpp"
