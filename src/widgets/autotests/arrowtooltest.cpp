/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "arrowtooltest.hpp"
#include "tools/arrowtool.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(ArrowToolTest)

ArrowToolTest::ArrowToolTest(QObject *parent)
    : QObject{parent}
{
}

void ArrowToolTest::shouldHaveDefaultValues()
{
    const ArrowTool t;
    QVERIFY(!t.tooltip().isEmpty());
    QCOMPARE(t.icon(), IconManager::Icon::TOOL_ARROW);

    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity
                                                    << Property::Type::StrokeStyle;
    QCOMPARE(t.properties().count(), 4);
    for (const auto &prop : properties) {
        QVERIFY(t.properties().contains(prop));
    }
}

#include "moc_arrowtooltest.cpp"
