/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "ellipsetooltest.hpp"
#include "tools/ellipsetool.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(EllipseToolTest)
using namespace Qt::Literals::StringLiterals;

EllipseToolTest::EllipseToolTest(QObject *parent)
    : QObject{parent}
{
}

void EllipseToolTest::shouldHaveDefaultValues()
{
    const EllipseTool t(nullptr);
    QCOMPARE(t.icon(), u"tool_ellipse"_s);
    QCOMPARE(t.type(), Tool::Type::Ellipse);

    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity
                                                    << Property::Type::StrokeStyle << Property::Type::BackgroundColor << Property::Type::BackgroundStyle;
    QCOMPARE(t.properties().count(), 6);
    for (const auto &prop : properties) {
        QVERIFY(t.properties().contains(prop));
    }
}

#include "moc_ellipsetooltest.cpp"
