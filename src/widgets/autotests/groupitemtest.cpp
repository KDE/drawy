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
    QCOMPARE(i.formType(), Item::FormType::Group);
    QVERIFY(!i.id().isEmpty());
    QVERIFY(!i.locked());
}

void GroupItemTest::shouldSerializeDefaultValue()
{
    GroupItem f;
    // Need to have an known id
    f.setId("acff679ae3c14260b56ef00f1d354883"_ba);
    const QJsonObject obj = f.serialize(-1); // Not define zorder yet
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

    std::shared_ptr<RectangleItem> r2 = std::make_shared<RectangleItem>();
    r2->setId("c9d6f90e757040e78dbf94bef5732e99"_ba);

    f.setItems({r, r2});

    const QJsonObject obj = f.serialize(-1); // Not define zorder yet

    GroupItem f2;
    f2.deserialize(obj);

    QCOMPARE(f2.items().count(), f.items().count());
    QCOMPARE(f2.items()[0]->id(), f.items()[0]->id());
    QCOMPARE(f2.items()[1]->id(), f.items()[1]->id());
}

void GroupItemTest::shouldTestTransformations()
{
    GroupItem i;
    QCOMPARE(i.transformObj(), QTransform());

    i.translate(QPointF(10, 20));
    QCOMPARE(i.transformObj(), QTransform().translate(10, 20));

    i.rotate(45, QPointF(5, 5));
    QTransform expected;
    expected.translate(10, 20);
    expected.translate(5, 5);
    expected.rotate(45);
    expected.translate(-5, -5);
    QCOMPARE(i.transformObj(), expected);

    i.resize(QTransform::fromScale(2, 2));
    expected = expected * QTransform::fromScale(2, 2);
    QCOMPARE(i.transformObj(), expected);

    i.setTransform(QTransform::fromScale(3, 3));
    QCOMPARE(i.transformObj(), QTransform::fromScale(3, 3));

    std::shared_ptr<RectangleItem> r = std::make_shared<RectangleItem>();
    r->setStart(QPointF(0, 0));
    r->setEnd(QPointF(10, 10));
    i.setItems({r});

    i.commitTransformation();
    QCOMPARE(i.transformObj(), QTransform());
    QCOMPARE(r->start(), QPointF(0, 0));
    QCOMPARE(r->end(), QPointF(30, 30));
    QCOMPARE(r->transformObj(), QTransform());
}

#include "moc_groupitemtest.cpp"
