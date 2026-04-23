/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "textitemtest.hpp"
#include "drawy_autotest_helper.hpp"
#include "item/text.hpp"
#include <QJsonObject>
#include <QTest>
QTEST_MAIN(TextItemTest)

using namespace Qt::Literals::StringLiterals;
TextItemTest::TextItemTest(QObject *parent)
    : QObject{parent}
{
}

void TextItemTest::shouldHaveDefaultValues()
{
    const TextItem i;
    QCOMPARE(i.formType(), Item::FormType::Text);
    const auto properties = QList<Property::Type>() << Property::Type::FontSize << Property::Type::StrokeColor << Property::Type::Opacity;
    QCOMPARE(i.allPropertyTypes().count(), 3);
    for (const auto &prop : properties) {
        QVERIFY(i.allPropertyTypes().contains(prop));
    }
    QVERIFY(!i.id().isEmpty());
    QVERIFY(!i.locked());
}

void TextItemTest::shouldSerializeDefaultValue()
{
    TextItem f;
    // Need to have an known id
    f.setId("acff679ae3c14260b56ef00f1d354883"_ba);
    const QJsonObject obj = f.serialize(-1); // Not define zorder yet
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/text/"_s, ba, u"defaultvalue"_s);
}

void TextItemTest::shouldTestTransformations()
{
    TextItem i;
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

    i.setProperty(Property::Type::FontSize, Property(10, Property::Type::FontSize));
    i.commitTransformation();
    QCOMPARE(i.property(Property::Type::FontSize).value<int>(), 30);
    QCOMPARE(i.transformObj(), QTransform());
}

void TextItemTest::shouldTestIntersects()
{
    TextItem i;
    i.createTextBox(QPointF(10, 10));
    i.insertText(u"Hello"_s);

    QVERIFY(i.intersects(QRectF(15, 15, 10, 10)));
    QVERIFY(!i.intersects(QRectF(0, 0, 5, 5)));
}

void TextItemTest::shouldRoundTrip()
{
    TextItem f;
    f.setId("acff679ae3c14260b56ef00f1d354883"_ba);
    f.createTextBox(QPointF(10, 10));
    f.insertText(u"Hello World"_s);

    const QJsonObject obj = f.serialize(-1);

    TextItem f2;
    f2.deserialize(obj);

    QCOMPARE(f2.text(), f.text());
    QCOMPARE(f2.boundingBox(), f.boundingBox());
}

void TextItemTest::shouldTestMode()
{
    TextItem i;
    QCOMPARE(i.mode(), TextItem::Mode::Normal);

    i.setMode(TextItem::Mode::Edit);
    QCOMPARE(i.mode(), TextItem::Mode::Edit);
}

void TextItemTest::shouldTestCaretAndSelection()
{
    TextItem i;
    i.insertText(u"Hello World"_s);

    QCOMPARE(i.caret(), 11);
    i.setCaret(5);
    QCOMPARE(i.caret(), 5);

    QVERIFY(!i.hasSelection());
    i.setSelectionStart(0);
    i.setSelectionEnd(5);
    QVERIFY(i.hasSelection());
    QCOMPARE(i.selectedText(), u"Hello"_s);

    i.deleteSelection();
    QCOMPARE(i.text(), u" World"_s);
    QCOMPARE(i.caret(), 0);
    QVERIFY(!i.hasSelection());
}

void TextItemTest::shouldTestTextManipulation()
{
    TextItem i;
    i.insertText(u"Hello"_s);
    QCOMPARE(i.text(), u"Hello"_s);

    i.setCaret(5);
    i.insertText(u" World"_s);
    QCOMPARE(i.text(), u"Hello World"_s);

    i.deleteSubStr(5, 10);
    QCOMPARE(i.text(), u"Hello"_s);
}

void TextItemTest::shouldTestLineRange()
{
    TextItem i;
    i.insertText(u"Line 1\nLine 2\nLine 3"_s);

    auto range1 = i.getLineRange(1);
    QCOMPARE(range1.first, 0);
    QCOMPARE(range1.second, 6);

    auto range2 = i.getLineRange(2);
    QCOMPARE(range2.first, 7);
    QCOMPARE(range2.second, 13);
}

void TextItemTest::shouldTestBreaks()
{
    TextItem i;
    i.insertText(u"Hello World Test"_s);

    QCOMPARE(i.getPrevBreak(11), 6); // Before 'W'
    QCOMPARE(i.getNextBreak(6), 11); // After 'd'
}

void TextItemTest::shouldTestNeedsCaching()
{
    const TextItem i;
    QVERIFY(i.needsCaching());
}

void TextItemTest::shouldTestLockAspectRatioWhenResizing()
{
    const TextItem i;
    QVERIFY(i.lockAspectRatioWhenResizing());
}

#include "moc_textitemtest.cpp"
