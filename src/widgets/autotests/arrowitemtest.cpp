/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "arrowitemtest.hpp"
#include "drawy_autotest_helper.hpp"
#include "item/arrow.hpp"
#include <QJsonObject>
#include <QTest>
QTEST_MAIN(ArrowItemTest)
using namespace Qt::Literals::StringLiterals;
ArrowItemTest::ArrowItemTest(QObject *parent)
    : QObject{parent}
{
}

void ArrowItemTest::shouldHaveDefaultValues()
{
    const ArrowItem i;
    QCOMPARE(i.type(), Item::Type::Arrow);
    QCOMPARE(i.startArrow(), ArrowItem::ArrowType::None);
    QCOMPARE(i.endArrow(), ArrowItem::ArrowType::Arrow);
    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity
                                                    << Property::Type::StrokeStyle;
    QCOMPARE(i.propertyTypes().count(), 4);
    for (const auto &prop : properties) {
        QVERIFY(i.propertyTypes().contains(prop));
    }
    QVERIFY(!i.id().isEmpty());
}

void ArrowItemTest::shouldSerializeDefaultValue()
{
    ArrowItem f;
    // Need to have an known id
    f.setId("b06576a8e6d648ba9b282c8a57fa8225"_ba);
    const QJsonObject obj = f.serialize();
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/arrow/"_s, ba, u"defaultvalue"_s);
}

void ArrowItemTest::shouldSerialize_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QPointF>("start");
    QTest::addColumn<QPointF>("end");
    QTest::addColumn<int>("strokeWidth");
    QTest::addColumn<QColor>("strokeColor");
    QTest::addColumn<QString>("strokeStyle");
    QTest::addRow("arrow1") << u"arrow1"_s << QPointF(-5.0, 5.0) << QPointF(10.0, 7.5) << 1 << QColor(Qt::red) << u"Solid"_s;
    QTest::addRow("arrow2") << u"arrow2"_s << QPointF(0.7, 5.0) << QPointF(8.0, 7.5) << 5 << QColor(Qt::blue) << u"DashLine"_s;
}

void ArrowItemTest::shouldSerialize()
{
    QFETCH(QString, name);
    QFETCH(QPointF, start);
    QFETCH(QPointF, end);
    QFETCH(int, strokeWidth);
    QFETCH(QColor, strokeColor);
    QFETCH(QString, strokeStyle);

    ArrowItem f;
    // Need to have an known id
    f.setId("4d8b0fe427a143a4b553399816007640"_ba);
    // Becarefull order ! start before end !
    f.setStart(start);
    f.setEnd(end);
    f.setProperty(Property::Type::StrokeWidth, Property(strokeWidth, Property::Type::StrokeWidth));
    f.setProperty(Property::Type::StrokeColor, Property(strokeColor, Property::Type::StrokeColor));
    f.setProperty(Property::Type::StrokeStyle, Property(strokeStyle, Property::Type::StrokeStyle));
    const QJsonObject obj = f.serialize();
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/arrow/"_s, ba, name);
}
#include "moc_arrowitemtest.cpp"
