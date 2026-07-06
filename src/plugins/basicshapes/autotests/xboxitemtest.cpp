/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "xboxitemtest.hpp"
#include "xbox/xbox.hpp"
#include <QTest>
QTEST_MAIN(XBoxItemTest)

XBoxItemTest::XBoxItemTest(QObject *parent)
    : QObject{parent}
{
}

void XBoxItemTest::shouldHaveDefaultValues()
{
    const XBoxItem i;
    QCOMPARE(i.formType(), Item::FormType::Custom);
    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity
                                                    << Property::Type::StrokeStyle << Property::Type::BackgroundColor << Property::Type::BackgroundStyle
                                                    << Property::Type::CornerRectangleType;
    QCOMPARE(i.allPropertyTypes().count(), 7);
    for (const auto &prop : properties) {
        QVERIFY(i.allPropertyTypes().contains(prop));
    }
    QVERIFY(!i.id().isEmpty());
    QVERIFY(!i.locked());
}
#include "moc_xboxitemtest.cpp"
