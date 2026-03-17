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

void GroupItemTest::shouldSerializeGroupWithItems()
{
    GroupItem f;
    f.setId("acff679ae3c14260b56ef00f1d354883"_ba);

    std::shared_ptr<RectangleItem> r = std::make_shared<RectangleItem>();
    r->setId("c9d6f90e757040e78dbf94bef5732e98"_ba);
    r->setStart(QPointF(10, 10));
    r->setEnd(QPointF(20, 20));

    std::shared_ptr<RectangleItem> r2 = std::make_shared<RectangleItem>();
    r2->setId("c9d6f90e757040e78dbf94bef5732e99"_ba);
    r2->setStart(QPointF(30, 30));
    r2->setEnd(QPointF(40, 40));

    f.group({r, r2});

    const QJsonObject obj = f.serialize(-1);
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/group/"_s, ba, u"groupwithitems"_s);
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

void GroupItemTest::shouldGroupItems()
{
    GroupItem i;
    std::shared_ptr<RectangleItem> r1 = std::make_shared<RectangleItem>();
    r1->setStart(QPointF(10, 10));
    r1->setEnd(QPointF(20, 20));

    std::shared_ptr<RectangleItem> r2 = std::make_shared<RectangleItem>();
    r2->setStart(QPointF(30, 30));
    r2->setEnd(QPointF(40, 40));

    i.group({r1, r2});

    QCOMPARE(i.items().count(), 2);
    QCOMPARE(i.transformObj(), QTransform().translate(9, 9));
    QCOMPARE(r1->start(), QPointF(10, 10));
    QCOMPARE(r1->end(), QPointF(20, 20));
    QCOMPARE(r1->transformObj(), QTransform().translate(-9, -9));
    QCOMPARE(r2->start(), QPointF(30, 30));
    QCOMPARE(r2->end(), QPointF(40, 40));
    QCOMPARE(r2->transformObj(), QTransform().translate(-9, -9));
}

void GroupItemTest::shouldUnGroupItems()
{
    GroupItem i;
    std::shared_ptr<RectangleItem> r1 = std::make_shared<RectangleItem>();
    r1->setStart(QPointF(10, 10));
    r1->setEnd(QPointF(20, 20));

    std::shared_ptr<RectangleItem> r2 = std::make_shared<RectangleItem>();
    r2->setStart(QPointF(30, 30));
    r2->setEnd(QPointF(40, 40));

    i.group({r1, r2});
    i.translate(QPointF(5, 5));

    QList<std::shared_ptr<Item>> items = i.unGroup();

    QCOMPARE(items.count(), 2);
    QCOMPARE(items[0]->transformObj(), QTransform().translate(5, 5));
    QCOMPARE(items[1]->transformObj(), QTransform().translate(5, 5));
}

void GroupItemTest::shouldTestIntersects()
{
    GroupItem i;
    std::shared_ptr<RectangleItem> r1 = std::make_shared<RectangleItem>();
    r1->setStart(QPointF(10, 10));
    r1->setEnd(QPointF(20, 20));

    std::shared_ptr<RectangleItem> r2 = std::make_shared<RectangleItem>();
    r2->setStart(QPointF(30, 30));
    r2->setEnd(QPointF(40, 40));

    i.group({r1, r2});

    QVERIFY(i.intersects(QRectF(15, 15, 5, 5)));
    QVERIFY(i.intersects(QRectF(35, 35, 5, 5)));
    QVERIFY(!i.intersects(QRectF(25, 25, 2, 2)));
}

void GroupItemTest::shouldTestBoundingBox()
{
    GroupItem i;
    std::shared_ptr<RectangleItem> r1 = std::make_shared<RectangleItem>();
    r1->setStart(QPointF(10, 10));
    r1->setEnd(QPointF(20, 20));

    std::shared_ptr<RectangleItem> r2 = std::make_shared<RectangleItem>();
    r2->setStart(QPointF(30, 30));
    r2->setEnd(QPointF(40, 40));

    i.group({r1, r2});

    QCOMPARE(i.boundingBox(), QRectF(9, 9, 32, 32));
    QCOMPARE(i.normalizedBoundingBox(), QRectF(0, 0, 32, 32));
    QCOMPARE(i.displayBoundingBox(), QPolygonF(QRectF(9, 9, 32, 32)));
}

void GroupItemTest::shouldTestProperties()
{
    GroupItem i;
    std::shared_ptr<RectangleItem> r1 = std::make_shared<RectangleItem>();
    r1->setProperty(Property::Type::StrokeColor, Property(QColor(Qt::red), Property::Type::StrokeColor));

    std::shared_ptr<RectangleItem> r2 = std::make_shared<RectangleItem>();
    r2->setProperty(Property::Type::StrokeColor, Property(QColor(Qt::red), Property::Type::StrokeColor));

    i.group({r1, r2});

    QCOMPARE(i.property(Property::Type::StrokeColor).value<QColor>(), QColor(Qt::red));

    r2->setProperty(Property::Type::StrokeColor, Property(QColor(Qt::blue), Property::Type::StrokeColor));
    QCOMPARE(i.property(Property::Type::StrokeColor).type(), Property::Type::StrokeColor);

    i.setProperty(Property::Type::StrokeColor, Property(QColor(Qt::green), Property::Type::StrokeColor));
    QCOMPARE(r1->property(Property::Type::StrokeColor).value<QColor>(), QColor(Qt::green));
    QCOMPARE(r2->property(Property::Type::StrokeColor).value<QColor>(), QColor(Qt::green));
    QCOMPARE(i.property(Property::Type::StrokeColor).value<QColor>(), QColor(Qt::green));

    QCOMPARE(i.allPropertyTypes().count(), r1->allPropertyTypes().count());
}

void GroupItemTest::shouldTestNeedsCaching()
{
    GroupItem i;
    std::shared_ptr<RectangleItem> r1 = std::make_shared<RectangleItem>();
    std::shared_ptr<RectangleItem> r2 = std::make_shared<RectangleItem>();

    i.group({r1, r2});
    QVERIFY(!i.needsCaching());
}

void GroupItemTest::shouldTestLockAspectRatioWhenResizing()
{
    GroupItem i;
    QVERIFY(i.lockAspectRatioWhenResizing());
}

#include "moc_groupitemtest.cpp"
