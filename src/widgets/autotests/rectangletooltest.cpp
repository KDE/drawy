/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "rectangletooltest.hpp"
#include "tools/rectangletool.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(RectangleToolTest)
using namespace Qt::Literals::StringLiterals;

RectangleToolTest::RectangleToolTest(QObject *parent)
    : QObject{parent}
{
}

void RectangleToolTest::shouldHaveDefaultValues()
{
    const RectangleTool t(nullptr);
    QVERIFY(!t.tooltip().isEmpty());
    QCOMPARE(t.icon(), u"tool_rectangle"_s);
    QCOMPARE(t.type(), Tool::Type::Rectangle);

    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity
                                                    << Property::Type::StrokeStyle << Property::Type::BackgroundColor << Property::Type::BackgroundStyle;
    QCOMPARE(t.properties().count(), 6);
    for (const auto &prop : properties) {
        QVERIFY(t.properties().contains(prop));
    }
}

#include "moc_rectangletooltest.cpp"
