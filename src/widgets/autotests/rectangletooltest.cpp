/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "rectangletooltest.hpp"
#include "tools/rectangletool.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(RectangleToolTest)

RectangleToolTest::RectangleToolTest(QObject *parent)
    : QObject{parent}
{
}

void RectangleToolTest::shouldHaveDefaultValues()
{
    const RectangleTool t;
    QVERIFY(!t.tooltip().isEmpty());
    QCOMPARE(t.icon(), IconManager::Icon::TOOL_RECTANGLE);

    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity
                                                    << Property::Type::StrokeStyle << Property::Type::BackgroundColor;
    QCOMPARE(t.properties().count(), 5);
    for (const auto &prop : properties) {
        QVERIFY(t.properties().contains(prop));
    }
}

#include "moc_rectangletooltest.cpp"
