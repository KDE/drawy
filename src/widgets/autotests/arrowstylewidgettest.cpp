/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "arrowstylewidgettest.hpp"
#include "properties/widgets/arrowstylewidget.hpp"
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
    const ArrowStyleWidget w(nullptr, parent);
}

#include "moc_arrowstylewidgettest.cpp"
