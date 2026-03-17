/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "rectangleitemtest.hpp"
#include "drawy_autotest_helper.hpp"
#include "item/rectangle.hpp"
#include "properties/property.hpp"
#include <QJsonObject>
#include <QTest>

QTEST_MAIN(RectangleItemTest)
using namespace Qt::Literals::StringLiterals;
RectangleItemTest::RectangleItemTest(QObject *parent)
    : QObject{parent}
{
}

void RectangleItemTest::shouldHaveDefaultValues()
{
    const RectangleItem i;
    QCOMPARE(i.formType(), Item::FormType::Rectangle);
    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity
                                                    << Property::Type::StrokeStyle << Property::Type::BackgroundColor << Property::Type::BackgroundStyle;
    QCOMPARE(i.allPropertyTypes().count(), 6);
    for (const auto &prop : properties) {
        QVERIFY(i.allPropertyTypes().contains(prop));
    }
    QVERIFY(!i.id().isEmpty());
    QVERIFY(!i.locked());
}

void RectangleItemTest::shouldSerializeDefaultValue()
{
    RectangleItem f;
    // Need to have an known id
    f.setId("acff679ae3c14260b56ef00f1d354883"_ba);
    const QJsonObject obj = f.serialize(-1); // Not define zorder yet
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/rectangle/"_s, ba, u"defaultvalue"_s);
}

void RectangleItemTest::shouldSerialize_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QPointF>("start");
    QTest::addColumn<QPointF>("end");
    QTest::addColumn<int>("strokeWidth");
    QTest::addColumn<QColor>("strokeColor");
    QTest::addColumn<bool>("locked");
    QTest::addColumn<QColor>("backgroundColor");
    QTest::addColumn<int>("zorder");
    QTest::addRow("rectangle1") << u"rectangle1"_s << QPointF(0.0, 5.0) << QPointF(10.0, 7.5) << 1 << QColor(Qt::red) << false << QColor(Qt::transparent) << 9;
    QTest::addRow("rectangle2") << u"rectangle2"_s << QPointF(0.2, 5.0) << QPointF(8.0, 7.5) << 5 << QColor(Qt::blue) << false << QColor(Qt::red) << 7;
    QTest::addRow("rectangle-locked1") << u"rectangle-locked1"_s << QPointF(0.2, 5.0) << QPointF(8.0, 7.5) << 5 << QColor(Qt::blue) << true << QColor(Qt::blue)
                                       << 3;
}

void RectangleItemTest::shouldSerialize()
{
    QFETCH(QString, name);
    QFETCH(QPointF, start);
    QFETCH(QPointF, end);
    QFETCH(int, strokeWidth);
    QFETCH(QColor, strokeColor);
    QFETCH(bool, locked);
    QFETCH(QColor, backgroundColor);
    QFETCH(int, zorder);

    RectangleItem f;
    // Need to have an known id
    f.setId("063856fce1c94095a8dc16a7c4842e5a"_ba);
    // Becarefull order ! start before end !
    f.setStart(start);
    f.setEnd(end);
    f.setProperty(Property::Type::StrokeWidth, Property(strokeWidth, Property::Type::StrokeWidth));
    f.setProperty(Property::Type::StrokeColor, Property(strokeColor, Property::Type::StrokeColor));
    f.setLocked(locked);
    f.setProperty(Property::Type::BackgroundColor, Property(backgroundColor, Property::Type::BackgroundColor));
    const QJsonObject obj = f.serialize(zorder);
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/rectangle/"_s, ba, name);
}

void RectangleItemTest::shouldDeserialize_data()
{
    QTest::addColumn<QString>("name");
    QTest::addRow("rectangle1") << u"rectangle1"_s;
    QTest::addRow("rectangle-locked1") << u"rectangle-locked1"_s;
}

void RectangleItemTest::shouldDeserialize()
{
    QFETCH(QString, name);
    RectangleItem f;
    const QString refFile = QLatin1StringView(DRAWY_GUI_DATA_DIR) + u"/rectangle/"_s + name + u".ref"_s;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(refFile);
    f.deserialize(obj);
    const QJsonObject deserializeObj = f.serialize(-1); // Not define zorder yet

    RectangleItem f2;
    f2.deserialize(deserializeObj);
    QCOMPARE(f, f2);
}

void RectangleItemTest::shouldTestTransformations()
{
    RectangleItem i;
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

void RectangleItemTest::shouldTestIntersects()
{
    RectangleItem i;
    i.setStart(QPointF(10, 10));
    i.setEnd(QPointF(30, 30));

    QVERIFY(i.intersects(QRectF(10, 10, 10, 10))); // Intersects the border
    QVERIFY(!i.intersects(QRectF(15, 15, 5, 5))); // Completely inside, not filled
    QVERIFY(!i.intersects(QRectF(0, 0, 5, 5))); // Completely outside
}

void RectangleItemTest::shouldTestOperatorEqual()
{
    RectangleItem i1;
    i1.setId("acff679ae3c14260b56ef00f1d354553"_ba);
    i1.setStart(QPointF(10, 10));
    i1.setEnd(QPointF(30, 30));

    RectangleItem i2;
    i2.setId("acff679ae3c14260b56ef00f1d354553"_ba);
    i2.setStart(QPointF(10, 10));
    i2.setEnd(QPointF(30, 30));

    QCOMPARE(i1, i2);

    i2.setStart(QPointF(0, 0));
    QVERIFY(!(i1 == i2));
}

#include "moc_rectangleitemtest.cpp"
