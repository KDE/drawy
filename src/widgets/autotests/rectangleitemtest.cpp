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
    QCOMPARE(i.type(), Item::Type::Rectangle);
    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity
                                                    << Property::Type::StrokeStyle;
    QCOMPARE(i.propertyTypes().count(), 4);
    for (const auto &prop : properties) {
        QVERIFY(i.propertyTypes().contains(prop));
    }
}

void RectangleItemTest::shouldSerializeDefaultValue()
{
    const RectangleItem f;
    const QJsonObject obj = f.serialize();
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
    QTest::addRow("rectangle1") << u"rectangle1"_s << QPointF(0.0, 5.0) << QPointF(10.0, 7.5) << 1 << QColor(Qt::red);
    QTest::addRow("rectangle2") << u"rectangle2"_s << QPointF(0.2, 5.0) << QPointF(8.0, 7.5) << 5 << QColor(Qt::blue);
}

void RectangleItemTest::shouldSerialize()
{
    QFETCH(QString, name);
    QFETCH(QPointF, start);
    QFETCH(QPointF, end);
    QFETCH(int, strokeWidth);
    QFETCH(QColor, strokeColor);

    RectangleItem f;
    // Becarefull order ! start before end !
    f.setStart(start);
    f.setEnd(end);
    f.setProperty(Property::Type::StrokeWidth, Property(strokeWidth, Property::Type::StrokeWidth));
    f.setProperty(Property::Type::StrokeColor, Property(strokeColor, Property::Type::StrokeColor));
    const QJsonObject obj = f.serialize();
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/rectangle/"_s, ba, name);
}

void RectangleItemTest::shouldDeserialize_data()
{
    QTest::addColumn<QString>("name");
    QTest::addRow("rectangle1") << u"rectangle1"_s;
}
void RectangleItemTest::shouldDeserialize()
{
    QFETCH(QString, name);
    RectangleItem f;
    const QString refFile = QLatin1StringView(DRAWY_DATA_DIR) + u"/rectangle/"_s + name + u".ref"_s;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(refFile);
    f.deserialize(obj);
    const QJsonObject deserializeObj = f.serialize();

    RectangleItem f2;
    f2.deserialize(deserializeObj);
    QCOMPARE(f, f2);
}

#include "moc_rectangleitemtest.cpp"
