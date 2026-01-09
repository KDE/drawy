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
    const auto properties = QVector<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity;
    QCOMPARE(i.propertyTypes().count(), 3);
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
    QTest::addRow("rectangle1") << u"rectangle1"_s << QPointF(0.0, 5.0) << QPointF(10.0, 7.5);
}

void RectangleItemTest::shouldSerialize()
{
    QFETCH(QString, name);
    QFETCH(QPointF, start);
    QFETCH(QPointF, end);

    RectangleItem f;
    // Becarefull order ! start before end !
    f.setStart(start);
    f.setEnd(end);
    const QJsonObject obj = f.serialize();
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/rectangle/"_s, ba, name);
}

#include "moc_rectangleitemtest.cpp"
