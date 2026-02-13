/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "triangleitemtest.hpp"
#include "triangle/triangle.hpp"
#include <QTest>
QTEST_MAIN(TriangleItemTest)

TriangleItemTest::TriangleItemTest(QObject *parent)
    : QObject{parent}
{
}

void TriangleItemTest::shouldHaveDefaultValues()
{
    const TriangleItem i;
    QCOMPARE(i.formType(), Item::FormType::Custom);
    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity
                                                    << Property::Type::StrokeStyle << Property::Type::BackgroundColor << Property::Type::BackgroundStyle;
    QCOMPARE(i.propertyTypes().count(), 6);
    for (const auto &prop : properties) {
        QVERIFY(i.propertyTypes().contains(prop));
    }
    QVERIFY(!i.id().isEmpty());
    QVERIFY(!i.locked());
}
