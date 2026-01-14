/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "groupitemtest.hpp"
#include "drawy_autotest_helper.hpp"
#include "item/group.hpp"
#include <QJsonObject>
#include <QTest>
QTEST_MAIN(GroupItemTest)
using namespace Qt::Literals::StringLiterals;
GroupItemTest::GroupItemTest(QObject *parent)
    : QObject{parent}
{
}

void GroupItemTest::shouldHaveDefaultValues()
{
    const GroupItem i;
    QCOMPARE(i.type(), Item::Type::Group);
    QVERIFY(!i.id().isEmpty());
}

void GroupItemTest::shouldSerializeDefaultValue()
{
    GroupItem f;
    // Need to have an known id
    f.setId("acff679ae3c14260b56ef00f1d354883"_ba);
    const QJsonObject obj = f.serialize();
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/group/"_s, ba, u"defaultvalue"_s);
}

#include "moc_groupitemtest.cpp"
