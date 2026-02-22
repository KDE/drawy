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
    QCOMPARE(i.formType(), Item::FormType::Ellipse);
    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity
                                                    << Property::Type::StrokeStyle << Property::Type::BackgroundColor << Property::Type::BackgroundStyle
                                                    << Property::Type::BackgroundStyle;
    QCOMPARE(i.propertyTypes().count(), 6);
    for (const auto &prop : properties) {
        QVERIFY(i.propertyTypes().contains(prop));
    }
    QVERIFY(!i.id().isEmpty());
    QVERIFY(!i.locked());
}

void EllipseItemTest::shouldSerializeDefaultValue()
{
    EllipseItem f;
    // Need to have an known id
    f.setId("acff679ae3c14260b56ef00f1d354883"_ba);
    const QJsonObject obj = f.serialize(-1); // Not define zorder yet
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/ellipse/"_s, ba, u"defaultvalue"_s);
}

void EllipseItemTest::shouldSerialize_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QPointF>("start");
    QTest::addColumn<QPointF>("end");
    QTest::addColumn<int>("strokeWidth");
    QTest::addColumn<QColor>("strokeColor");
    QTest::addColumn<bool>("locked");
    QTest::addRow("ellipse1") << u"ellipse1"_s << QPointF(-5.0, 5.0) << QPointF(10.0, 7.5) << 1 << QColor(Qt::red) << true;
    QTest::addRow("ellipse2") << u"ellipse2"_s << QPointF(0.7, 5.0) << QPointF(8.0, 7.5) << 5 << QColor(Qt::blue) << false;
}

void EllipseItemTest::shouldSerialize()
{
    QFETCH(QString, name);
    QFETCH(QPointF, start);
    QFETCH(QPointF, end);
    QFETCH(int, strokeWidth);
    QFETCH(QColor, strokeColor);
    QFETCH(bool, locked);

    EllipseItem f;
    // Becarefull order ! start before end !
    f.setStart(start);
    f.setId("70a68950047e4a2797ee2f32b21cba61"_ba);
    f.setEnd(end);
    f.setProperty(Property::Type::StrokeWidth, Property(strokeWidth, Property::Type::StrokeWidth));
    f.setProperty(Property::Type::StrokeColor, Property(strokeColor, Property::Type::StrokeColor));
    f.setLocked(locked);
    const QJsonObject obj = f.serialize(-1); // Not define zorder yet
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/ellipse/"_s, ba, name);
}

void EllipseItemTest::shouldTestTransformations()
{
    EllipseItem i;
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

#include "moc_ellipseitemtest.cpp"
