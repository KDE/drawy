/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "freeformitemtest.hpp"
#include "drawy_autotest_helper.hpp"
#include "item/freeform.hpp"

#include <QJsonObject>
#include <QTest>
QTEST_MAIN(FreeformItemTest)
using namespace Qt::Literals::StringLiterals;
FreeformItemTest::FreeformItemTest(QObject *parent)
    : QObject{parent}
{
}

void FreeformItemTest::shouldHaveDefaultValues()
{
    const FreeformItem i;
    QCOMPARE(i.formType(), Item::FormType::Freeform);
    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::StrokeStyle
                                                    << Property::Type::Opacity;
    QCOMPARE(i.allPropertyTypes().count(), 4);
    for (const auto &prop : properties) {
        QVERIFY(i.allPropertyTypes().contains(prop));
    }
    QVERIFY(!i.id().isEmpty());
    QVERIFY(!i.locked());
}

void FreeformItemTest::shouldSerializeDefaultValue()
{
    FreeformItem f;
    // Need to have an known id
    f.setId("acff679ae3c14260b56ef00f1d354553"_ba);
    const QJsonObject obj = f.serialize(-1); // Not define zorder yet
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/freeform/"_s, ba, u"defaultvalue"_s);
}

void FreeformItemTest::shouldTestTransformations()
{
    FreeformItem i;
    QCOMPARE(i.transformObj(), QTransform());

    i.translate(QPointF(10, 20));
    QCOMPARE(i.transformObj(), QTransform().translate(10, 20));

    i.rotate(45, QPointF(5, 5));
    QTransform expected;
    expected.translate(10, 20);
    expected.translate(5, 5);
    expected.rotate(45);
    expected.translate(-5, -5);
    QCOMPARE(i.transformObj(), expected);

    i.resize(QTransform::fromScale(2, 2));
    expected = expected * QTransform::fromScale(2, 2);
    QCOMPARE(i.transformObj(), expected);

    i.setTransform(QTransform::fromScale(3, 3));
    QCOMPARE(i.transformObj(), QTransform::fromScale(3, 3));

    i.addPoint(QPointF(0, 0), 1.0);
    i.addPoint(QPointF(10, 10), 1.0);
    i.finalizeStroke();
    i.commitTransformation();
    QCOMPARE(i.points().at(0), QPointF(0, 0));
    QCOMPARE(i.points().at(1), QPointF(30, 30));
    QCOMPARE(i.transformObj(), QTransform());
}

void FreeformItemTest::shouldTestIntersects()
{
    FreeformItem i;
    i.addPoint(QPointF(10, 10), 1.0);
    i.addPoint(QPointF(30, 30), 1.0);
    i.finalizeStroke();

    QVERIFY(i.intersects(QRectF(15, 15, 10, 10)));
    QVERIFY(!i.intersects(QRectF(0, 0, 5, 5)));
}

void FreeformItemTest::shouldRoundTrip()
{
    FreeformItem f;
    f.setId("acff679ae3c14260b56ef00f1d354553"_ba);
    f.addPoint(QPointF(10, 10), 1.0);
    f.addPoint(QPointF(20, 20), 1.0);
    f.finalizeStroke();

    const QJsonObject obj = f.serialize(-1);

    FreeformItem f2;
    f2.deserialize(obj);

    QCOMPARE(f2.points().count(), f.points().count());
    QCOMPARE(f2.points().at(0), f.points().at(0));
    QCOMPARE(f2.points().at(1), f.points().at(1));
}

void FreeformItemTest::shouldTestPressureSimulation()
{
    FreeformItem i;
    QVERIFY(i.isPressureSimulated());

    i.setSimulatePressure(false);
    QVERIFY(!i.isPressureSimulated());
}

void FreeformItemTest::shouldTestNeedsCaching()
{
    FreeformItem i;
    QVERIFY(i.needsCaching());
}

void FreeformItemTest::shouldTestTransformHandlers()
{
    FreeformItem i;
    i.addPoint(QPointF(10, 10), 1.0);
    i.finalizeStroke();
    QCOMPARE(i.transformHandlers().count(), 1);
    QCOMPARE(i.transformHandlers().first(), TransformHandlerUtils::Type::MoveTransformHandler);

    FreeformItem i2;
    i2.addPoint(QPointF(10, 10), 1.0);
    i2.addPoint(QPointF(20, 20), 1.0);
    i2.finalizeStroke();
    QVERIFY(i2.transformHandlers().count() > 1);
}

#include "moc_freeformitemtest.cpp"
