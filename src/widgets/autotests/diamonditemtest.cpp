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
    const QJsonObject obj = f.serialize();
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/diamond/"_s, ba, u"defaultvalue"_s);
}

#include "moc_diamonditemtest.cpp"
