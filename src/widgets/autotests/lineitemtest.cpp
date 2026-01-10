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
    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity;
    QCOMPARE(i.propertyTypes().count(), 3);
    for (const auto &prop : properties) {
        QVERIFY(i.propertyTypes().contains(prop));
    }
}

void LineItemTest::shouldSerializeDefaultValue()
{
    const LineItem f;
    const QJsonObject obj = f.serialize();
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/line/"_s, ba, u"defaultvalue"_s);
}
#include "moc_lineitemtest.cpp"
