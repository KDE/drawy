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
}

#include "moc_backgroundstylewidgettest.cpp"
