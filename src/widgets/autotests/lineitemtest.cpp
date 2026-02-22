/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "lineitemtest.hpp"
#include "drawy_autotest_helper.hpp"
#include "item/line.hpp"
#include <QJsonObject>
#include <QTest>
QTEST_MAIN(LineItemTest)
using namespace Qt::Literals::StringLiterals;
LineItemTest::LineItemTest(QObject *parent)
    : QObject{parent}
{
}

void LineItemTest::shouldHaveDefaultValues()
{
    const LineItem i;
    QCOMPARE(i.formType(), Item::FormType::Line);
    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity
                                                    << Property::Type::StrokeStyle;
    QCOMPARE(i.propertyTypes().count(), 4);
    for (const auto &prop : properties) {
        QVERIFY(i.propertyTypes().contains(prop));
    }
    QVERIFY(!i.id().isEmpty());
    QVERIFY(!i.locked());
}

void LineItemTest::shouldSerializeDefaultValue()
{
    LineItem f;
    // Need to have an known id
    f.setId("167ce602c9a34f2692304328e8dc03f0"_ba);
    const QJsonObject obj = f.serialize(-1); // Not define zorder yet
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/line/"_s, ba, u"defaultvalue"_s);
}

void LineItemTest::shouldSerialize_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QPointF>("start");
    QTest::addColumn<QPointF>("end");
    QTest::addColumn<int>("strokeWidth");
    QTest::addColumn<QColor>("strokeColor");
    QTest::addColumn<bool>("locked");
    QTest::addColumn<int>("zorder");
    QTest::addRow("line1") << u"line1"_s << QPointF(-5.0, 5.0) << QPointF(10.0, 7.5) << 1 << QColor(Qt::red) << false << 5;
    QTest::addRow("line2") << u"line2"_s << QPointF(0.7, 5.0) << QPointF(8.0, 7.5) << 5 << QColor(Qt::blue) << true << 10;
}

void LineItemTest::shouldSerialize()
{
    QFETCH(QString, name);
    QFETCH(QPointF, start);
    QFETCH(QPointF, end);
    QFETCH(int, strokeWidth);
    QFETCH(QColor, strokeColor);
    QFETCH(bool, locked);
    QFETCH(int, zorder);

    LineItem f;
    // Need to have an known id
    f.setId("fb0ba748fee64b4a89de76d94787f73e"_ba);
    // Becarefull order ! start before end !
    f.setStart(start);
    f.setEnd(end);
    f.setProperty(Property::Type::StrokeWidth, Property(strokeWidth, Property::Type::StrokeWidth));
    f.setProperty(Property::Type::StrokeColor, Property(strokeColor, Property::Type::StrokeColor));
    f.setLocked(locked);
    const QJsonObject obj = f.serialize(zorder);
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/line/"_s, ba, name);
}

void LineItemTest::shouldTestTransformations()
{
    LineItem i;
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

    i.setStart(QPointF(0, 0));
    i.setEnd(QPointF(10, 10));
    i.commitTransformation();
    QCOMPARE(i.start(), QPointF(0, 0));
    QCOMPARE(i.end(), QPointF(30, 30));
    QCOMPARE(i.transformObj(), QTransform());
}

void LineItemTest::shouldTestIntersects()
{
    LineItem i;
    i.setStart(QPointF(10, 10));
    i.setEnd(QPointF(30, 30));

    QVERIFY(i.intersects(QRectF(15, 15, 10, 10)));
    QVERIFY(!i.intersects(QRectF(0, 0, 5, 5)));
}

void LineItemTest::shouldRoundTrip()
{
    LineItem f;
    f.setId("fb0ba748fee64b4a89de76d94787f73e"_ba);
    f.setStart(QPointF(10, 10));
    f.setEnd(QPointF(20, 20));

    const QJsonObject obj = f.serialize(-1);

    LineItem f2;
    f2.deserialize(obj);

    QCOMPARE(f2.start(), f.start());
    QCOMPARE(f2.end(), f.end());
}

void LineItemTest::shouldTestSetEndWithShift()
{
    LineItem i;
    i.setStart(QPointF(0, 0));
    i.setEndWithShift(QPointF(10, 2)); // Should snap to horizontal
    QCOMPARE(i.end().y(), 0.0);
}

void LineItemTest::shouldTestTransformHandlers()
{
    LineItem i;
    QCOMPARE(i.transformHandlers().count(), 2);
    QVERIFY(i.transformHandlers().contains(TransformHandler::Type::LineResizeTransformHandler));
    QVERIFY(i.transformHandlers().contains(TransformHandler::Type::LineMoveTransformHandler));
}

#include "moc_lineitemtest.cpp"
