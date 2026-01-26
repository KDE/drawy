/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "alignmentwidgettest.hpp"
#include "properties/widgets/alignmentwidget.hpp"
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
    const AlignmentWidget w(parent);
    QVERIFY(!w.name().isEmpty());
    auto m_widget = parent->findChild<QWidget *>(u"m_widget"_s);
    QVERIFY(m_widget);

    auto layout = parent->findChild<QGridLayout *>(u"layout"_s);
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins{});
}

#include "moc_alignmentwidgettest.cpp"
