/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "groupitemtest.hpp"
#include "drawy_autotest_helper.hpp"
#include "item/group.hpp"
#include "item/rectangle.hpp"
#include <QJsonObject>
#include <QTest>
#include <memory>
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
    QVERIFY(!i.locked());
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

void GroupItemTest::shouldRoundTripItems()
{
    GroupItem f;
    // Need to have an known id
    f.setId("acff679ae3c14260b56ef00f1d354883"_ba);

    std::shared_ptr<RectangleItem> r = std::make_shared<RectangleItem>();
    r->setId("c9d6f90e757040e78dbf94bef5732e98"_ba);

    f.setItems({r});

    const QJsonObject obj = f.serialize();

    GroupItem f2;
    f2.deserialize(obj);

    QCOMPARE(f2.items().length(), f.items().length());
    QCOMPARE(f2.items()[0]->id(), f.items()[0]->id());
}

#include "moc_groupitemtest.cpp"
