/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "arrowbuttonactionswidgettest.hpp"
#include "properties/widgets/arrowbuttonactionswidget.hpp"
#include <QTest>

QTEST_MAIN(ArrowButtonActionsWidgetTest)

ArrowButtonActionsWidgetTest::ArrowButtonActionsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ArrowButtonActionsWidgetTest::shouldHaveDefaultValues()
{
    const ArrowButtonActionsWidget w;
    // TODO
}

#include "moc_arrowbuttonactionswidgettest.cpp"
