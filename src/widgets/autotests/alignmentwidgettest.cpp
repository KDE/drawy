/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "alignmentwidgettest.hpp"
#include "properties/widgets/alignmentwidget.hpp"
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
}
