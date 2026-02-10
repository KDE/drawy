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
    QCOMPARE(i.type(), Item::Type::Diamond);
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

#include "moc_diamonditemtest.cpp"
