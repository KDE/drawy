/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "diamondtooltest.hpp"
#include "tools/diamondtool.hpp"
#include <QTest>

QTEST_GUILESS_MAIN(DiamondToolTest)

using namespace Qt::Literals::StringLiterals;
DiamondToolTest::DiamondToolTest(QObject *parent)
    : QObject{parent}
{
}

void DiamondToolTest::shouldHaveDefaultValues()
{
    const DiamondTool t(nullptr);
    QCOMPARE(t.type(), Tool::Type::Diamond);

    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity
                                                    << Property::Type::StrokeStyle << Property::Type::BackgroundColor << Property::Type::BackgroundStyle;
    QCOMPARE(t.allProperties().count(), 6);
    for (const auto &prop : properties) {
        QVERIFY(t.allProperties().contains(prop));
    }
}

#include "moc_diamondtooltest.cpp"
