/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "customelementswidgettest.hpp"
#include "customelementswidget/customelementswidget.hpp"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(CustomElementsWidgetTest)
using namespace Qt::Literals::StringLiterals;
CustomElementsWidgetTest::CustomElementsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CustomElementsWidgetTest::shouldHaveDefaultValues()
{
    const CustomElementsWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
}
#include "moc_customelementswidgettest.cpp"
