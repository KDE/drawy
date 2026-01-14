/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "opacitywidgettest.hpp"
#include "properties/widgets/opacitywidget.hpp"
#include <QSlider>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(OpacityWidgetTest)

OpacityWidgetTest::OpacityWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void OpacityWidgetTest::shouldHaveDefaultValues()
{
    const OpacityWidget w;
    QVERIFY(!w.name().isEmpty());
    QVERIFY(w.widget());
    QCOMPARE(w.widget()->objectName(), u"slider"_s);
}

#include "moc_opacitywidgettest.cpp"
