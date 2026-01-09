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
    const auto properties = QVector<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity;
    QCOMPARE(i.propertyTypes().count(), 3);
    for (const auto &prop : properties) {
        QVERIFY(i.propertyTypes().contains(prop));
    }
}

void ArrowItemTest::shouldSerializeDefaultValue()
{
    const ArrowItem f;
    const QJsonObject obj = f.serialize();
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/arrow/"_s, ba, u"defaultvalue"_s);
}
#include "moc_arrowitemtest.cpp"
