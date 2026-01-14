/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "freeformitemtest.hpp"
#include "drawy_autotest_helper.hpp"
#include "item/freeform.hpp"

#include <QJsonObject>
#include <QTest>
QTEST_MAIN(FreeformItemTest)
using namespace Qt::Literals::StringLiterals;
FreeformItemTest::FreeformItemTest(QObject *parent)
    : QObject{parent}
{
}

void FreeformItemTest::shouldHaveDefaultValues()
{
    const FreeformItem i;
    QCOMPARE(i.type(), Item::Type::Freeform);
    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity;
    QCOMPARE(i.propertyTypes().count(), 3);
    for (const auto &prop : properties) {
        QVERIFY(i.propertyTypes().contains(prop));
    }
    QVERIFY(!i.id().isEmpty());
}

void FreeformItemTest::shouldSerializeDefaultValue()
{
    FreeformItem f;
    // Need to have an known id
    f.setId("acff679ae3c14260b56ef00f1d354553"_ba);
    const QJsonObject obj = f.serialize();
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/freeform/"_s, ba, u"defaultvalue"_s);
}
#include "moc_freeformitemtest.cpp"
