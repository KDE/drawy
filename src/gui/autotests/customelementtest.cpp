/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "customelementtest.hpp"
#include "customelements/customelement.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(CustomElementTest)

CustomElementTest::CustomElementTest(QObject *parent)
    : QObject{parent}
{
}

void CustomElementTest::shouldHaveDefaultValues()
{
    const CustomElement w;
    QVERIFY(!w.isValid());
    QVERIFY(!w.item());
}
#include "moc_customelementtest.cpp"
