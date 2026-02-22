/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "diamonditemtest.hpp"
#include "drawy_autotest_helper.hpp"
#include "item/diamond.hpp"
#include <QJsonObject>
#include <QTest>
QTEST_MAIN(DiamondItemTest)
using namespace Qt::Literals::StringLiterals;
DiamondItemTest::DiamondItemTest(QObject *parent)
    : QObject{parent}
{
}

void DiamondItemTest::shouldHaveDefaultValues()
{
    const DiamondItem i;
    QCOMPARE(i.formType(), Item::FormType::Diamond);
    QVERIFY(!i.id().isEmpty());
    QVERIFY(!i.locked());
}

void DiamondItemTest::shouldSerializeDefaultValue()
{
    DiamondItem f;
    // Need to have an known id
    f.setId("acff679ae3c14260b56ef00f1d354883"_ba);
    const QJsonObject obj = f.serialize(-1); // Not define zorder yet
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/diamond/"_s, ba, u"defaultvalue"_s);
}

void DiamondItemTest::shouldSerialize_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QPointF>("start");
    QTest::addColumn<QPointF>("end");
    QTest::addColumn<int>("strokeWidth");
    QTest::addColumn<QColor>("strokeColor");
    QTest::addColumn<bool>("locked");
    QTest::addColumn<QColor>("backgroundColor");
    QTest::addColumn<int>("zorder");
    QTest::addRow("diamond1") << u"diamond1"_s << QPointF(0.0, 5.0) << QPointF(10.0, 7.5) << 1 << QColor(Qt::red) << false << QColor(Qt::transparent) << 9;
    QTest::addRow("diamond2") << u"diamond2"_s << QPointF(0.2, 5.0) << QPointF(8.0, 7.5) << 5 << QColor(Qt::blue) << false << QColor(Qt::red) << 7;
    QTest::addRow("diamond-locked1") << u"diamond-locked1"_s << QPointF(0.2, 5.0) << QPointF(8.0, 7.5) << 5 << QColor(Qt::blue) << true << QColor(Qt::blue)
                                     << 3;
}

void DiamondItemTest::shouldSerialize()
{
    QFETCH(QString, name);
    QFETCH(QPointF, start);
    QFETCH(QPointF, end);
    QFETCH(int, strokeWidth);
    QFETCH(QColor, strokeColor);
    QFETCH(bool, locked);
    QFETCH(QColor, backgroundColor);
    QFETCH(int, zorder);

    DiamondItem f;
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
    AutoTestHelper::compareFile(u"/diamond/"_s, ba, name);
}

void DiamondItemTest::shouldDeserialize_data()
{
    QTest::addColumn<QString>("name");
    QTest::addRow("diamond1") << u"diamond1"_s;
    QTest::addRow("diamond-locked1") << u"diamond-locked1"_s;
}

void DiamondItemTest::shouldDeserialize()
{
    QFETCH(QString, name);
    DiamondItem f;
    const QString refFile = QLatin1StringView(DRAWY_DATA_DIR) + u"/diamond/"_s + name + u".ref"_s;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(refFile);
    f.deserialize(obj);
    const QJsonObject deserializeObj = f.serialize(-1); // Not define zorder yet

    DiamondItem f2;
    f2.deserialize(deserializeObj);
    QCOMPARE(f, f2);
}

void DiamondItemTest::shouldTestTransformations()
{
    DiamondItem i;
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

void DiamondItemTest::shouldTestIntersects()
{
    DiamondItem i;
    i.setStart(QPointF(10, 10));
    i.setEnd(QPointF(30, 30));

    QVERIFY(i.intersects(QRectF(15, 15, 10, 10)));
    QVERIFY(!i.intersects(QRectF(0, 0, 5, 5)));
}

void DiamondItemTest::shouldTestOperatorEqual()
{
    DiamondItem i1;
    i1.setId("acff679ae3c14260b56ef00f1d354553"_ba);
    i1.setStart(QPointF(10, 10));
    i1.setEnd(QPointF(30, 30));

    DiamondItem i2;
    i2.setId("acff679ae3c14260b56ef00f1d354553"_ba);
    i2.setStart(QPointF(10, 10));
    i2.setEnd(QPointF(30, 30));

    QCOMPARE(i1, i2);

    i2.setStart(QPointF(0, 0));
    QVERIFY(!(i1 == i2));
}

#include "moc_diamonditemtest.cpp"
