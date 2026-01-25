/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "linetooltest.hpp"
#include "tools/linetool.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(LineToolTest)
using namespace Qt::Literals::StringLiterals;

LineToolTest::LineToolTest(QObject *parent)
    : QObject{parent}
{
}

void LineToolTest::shouldHaveDefaultValues()
{
    const LineTool t;
    QVERIFY(!t.tooltip().isEmpty());
    QCOMPARE(t.icon(), u"tool_line"_s);

    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity
                                                    << Property::Type::StrokeStyle;
    QCOMPARE(t.properties().count(), 4);
    for (const auto &prop : properties) {
        QVERIFY(t.properties().contains(prop));
    }
}

#include "moc_linetooltest.cpp"
