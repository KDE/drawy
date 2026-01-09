/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "ellipseitemtest.hpp"
#include "drawy_autotest_helper.hpp"
#include "item/ellipse.hpp"
#include <QJsonObject>
#include <QTest>
QTEST_MAIN(EllipseItemTest)
using namespace Qt::Literals::StringLiterals;
EllipseItemTest::EllipseItemTest(QObject *parent)
    : QObject{parent}
{
}

void EllipseItemTest::shouldHaveDefaultValues()
{
    const EllipseItem i;
    QCOMPARE(i.type(), Item::Type::Ellipse);
    const auto properties = QVector<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity;
    QCOMPARE(i.propertyTypes().count(), 3);
    for (const auto &prop : properties) {
        QVERIFY(i.propertyTypes().contains(prop));
    }
}

void EllipseItemTest::shouldSerializeDefaultValue()
{
    const EllipseItem f;
    const QJsonObject obj = f.serialize();
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/ellipse/"_s, ba, u"defaultvalue"_s);
}
#include "moc_ellipseitemtest.cpp"
