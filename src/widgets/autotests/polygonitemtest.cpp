/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "polygonitemtest.hpp"
#include "drawy_autotest_helper.hpp"
#include "item/polygon.hpp"
#include <QTest>
QTEST_MAIN(PolygonItemTest)
using namespace Qt::Literals::StringLiterals;
PolygonItemTest::PolygonItemTest(QObject *parent)
    : QObject{parent}
{
}

void PolygonItemTest::shouldHaveDefaultValues()
{
    PolygonItem i;
    QCOMPARE(i.start(), QPointF(0, 0));
    QCOMPARE(i.end(), QPointF(0, 0));
    QCOMPARE(i.propertyTypes().count(), 4);
    QVERIFY(i.propertyTypes().contains(Property::Type::StrokeWidth));
    QVERIFY(i.propertyTypes().contains(Property::Type::StrokeColor));
    QVERIFY(i.propertyTypes().contains(Property::Type::Opacity));
    QVERIFY(i.propertyTypes().contains(Property::Type::StrokeStyle));
}

void PolygonItemTest::shouldTestInitPoints()
{
    PolygonItem i;
    i.initPoints(QPointF(10, 10));
    QCOMPARE(i.start(), QPointF(10, 10));
    QCOMPARE(i.end(), QPointF(10, 10));
}

void PolygonItemTest::shouldTestSetPoints()
{
    PolygonItem i;
    i.setStart(QPointF(10, 10));
    i.setEnd(QPointF(20, 20));
    QCOMPARE(i.start(), QPointF(10, 10));
    QCOMPARE(i.end(), QPointF(20, 20));
}

void PolygonItemTest::shouldTestSetEndWithShift()
{
    PolygonItem i;
    i.setStart(QPointF(10, 10));
    i.setEndWithShift(QPointF(20, 15)); // dx=10, dy=5 -> max=10 -> end=(20, 20)
    QCOMPARE(i.end(), QPointF(20, 20));

    i.setEndWithShift(QPointF(5, 20)); // dx=-5, dy=10 -> max=10 -> end=(0, 20)
    QCOMPARE(i.end(), QPointF(0, 20));
}

void PolygonItemTest::shouldRoundTrip()
{
    PolygonItem f;
    f.setId("acff679ae3c14260b56ef00f1d354553"_ba);
    f.setStart(QPointF(10, 10));
    f.setEnd(QPointF(20, 20));

    const QJsonObject obj = f.serialize(-1);

    PolygonItem f2;
    f2.deserialize(obj);

    QCOMPARE(f2.start(), f.start());
    QCOMPARE(f2.end(), f.end());
    QVERIFY(f == f2);
}

void PolygonItemTest::shouldTestOperatorEqual()
{
    PolygonItem i1;
    i1.setStart(QPointF(10, 10));
    i1.setEnd(QPointF(20, 20));

    PolygonItem i2;
    i2.setStart(QPointF(10, 10));
    i2.setEnd(QPointF(20, 20));

    QVERIFY(i1 == i2);

    i2.setEnd(QPointF(30, 30));
    QVERIFY(!(i1 == i2));
}

void PolygonItemTest::shouldTestTransformations()
{
    PolygonItem i;
    i.setStart(QPointF(10, 10));
    i.setEnd(QPointF(20, 20));

    i.setTransform(QTransform::fromScale(2, 2));
    i.commitTransformation();

    QCOMPARE(i.start(), QPointF(20, 20));
    QCOMPARE(i.end(), QPointF(40, 40));
    QCOMPARE(i.transformObj(), QTransform());
}

#include "moc_polygonitemtest.cpp"
