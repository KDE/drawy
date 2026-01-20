/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "arrowstylewidgettest.hpp"
#include "properties/widgets/arrowstylewidget.hpp"
#include "properties/widgets/buttonactionswidget.hpp"
#include <QHBoxLayout>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(ArrowStyleWidgetTest)

ArrowStyleWidgetTest::ArrowStyleWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ArrowStyleWidgetTest::shouldHaveDefaultValues()
{
    auto parent = new QWidget();
    const ArrowStyleWidget w(parent);
}

#include "moc_arrowstylewidgettest.cpp"
