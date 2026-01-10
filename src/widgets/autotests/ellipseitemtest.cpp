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
    QCOMPARE(i.type(), Item::Type::Ellipse);
    const auto properties = QVector<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity;
    QCOMPARE(i.propertyTypes().count(), 3);
    for (const auto &prop : properties) {
        QVERIFY(i.propertyTypes().contains(prop));
    }
}

void EllipseItemTest::shouldSerializeDefaultValue()
{
    const EllipseItem f;
    const QJsonObject obj = f.serialize();
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
    QTest::addColumn<QString>("strokeColor");
    QTest::addRow("ellipse1") << u"ellipse1"_s << QPointF(-5.0, 5.0) << QPointF(10.0, 7.5) << 1 << u"#255641"_s;
    QTest::addRow("ellipse2") << u"ellipse2"_s << QPointF(0.7, 5.0) << QPointF(8.0, 7.5) << 5 << u"#FF00FF"_s;
}

void EllipseItemTest::shouldSerialize()
{
    QFETCH(QString, name);
    QFETCH(QPointF, start);
    QFETCH(QPointF, end);
    QFETCH(int, strokeWidth);
    QFETCH(QString, strokeColor);

    EllipseItem f;
    // Becarefull order ! start before end !
    f.setStart(start);
    f.setEnd(end);
    f.setProperty(Property::Type::StrokeWidth, Property(strokeWidth, Property::Type::StrokeWidth));
    f.setProperty(Property::Type::StrokeColor, Property(strokeColor, Property::Type::StrokeColor));
    const QJsonObject obj = f.serialize();
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/ellipse/"_s, ba, name);
}

#include "moc_ellipseitemtest.cpp"
