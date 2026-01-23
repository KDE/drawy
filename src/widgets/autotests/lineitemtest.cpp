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
    QCOMPARE(i.type(), Item::Type::Line);
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
    const QJsonObject obj = f.serialize();
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
    QTest::addRow("line1") << u"line1"_s << QPointF(-5.0, 5.0) << QPointF(10.0, 7.5) << 1 << QColor(Qt::red);
    QTest::addRow("line2") << u"line2"_s << QPointF(0.7, 5.0) << QPointF(8.0, 7.5) << 5 << QColor(Qt::blue);
}

void LineItemTest::shouldSerialize()
{
    QFETCH(QString, name);
    QFETCH(QPointF, start);
    QFETCH(QPointF, end);
    QFETCH(int, strokeWidth);
    QFETCH(QColor, strokeColor);

    LineItem f;
    // Need to have an known id
    f.setId("fb0ba748fee64b4a89de76d94787f73e"_ba);
    // Becarefull order ! start before end !
    f.setStart(start);
    f.setEnd(end);
    f.setProperty(Property::Type::StrokeWidth, Property(strokeWidth, Property::Type::StrokeWidth));
    f.setProperty(Property::Type::StrokeColor, Property(strokeColor, Property::Type::StrokeColor));
    const QJsonObject obj = f.serialize();
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/line/"_s, ba, name);
}
#include "moc_lineitemtest.cpp"
