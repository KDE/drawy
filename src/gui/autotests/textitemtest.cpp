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
    i.cursor().insertText(u"Hello"_s);

    QVERIFY(i.intersects(QRectF(15, 15, 10, 10)));
    QVERIFY(!i.intersects(QRectF(0, 0, 5, 5)));
}

void TextItemTest::shouldRoundTrip()
{
    TextItem f;
    f.setId("acff679ae3c14260b56ef00f1d354883"_ba);
    f.createTextBox(QPointF(10, 10));
    f.cursor().insertText(u"Hello World"_s);

    const QJsonObject obj = f.serialize(-1);

    TextItem f2;
    f2.deserialize(obj);

    QCOMPARE(f2.text(), f.text());
    QCOMPARE(f2.boundingBox(), f.boundingBox());
}

void TextItemTest::shouldTestLoadPlainText()
{
    TextItem f;
    f.setId("acff679ae3c14260b56ef00f1d354883"_ba);
    f.createTextBox(QPointF(10, 10));
    f.cursor().insertText(u"Hello World"_s);

    QJsonObject obj = f.serialize(-1);
    const auto plainText = u"Plain Text"_s;
    obj[u"text"_s] = plainText;

    TextItem f2;
    f2.deserialize(obj);

    QCOMPARE(f2.text(), plainText);
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
    i.cursor().insertText(u"Hello World"_s);

    QCOMPARE(i.cursor().position(), 11);
    i.cursor().setPosition(5);
    QCOMPARE(i.cursor().position(), 5);

    QVERIFY(!i.cursor().hasSelection());
    i.cursor().setPosition(0);
    i.cursor().setPosition(5, QTextCursor::KeepAnchor);
    QVERIFY(i.cursor().hasSelection());
    QCOMPARE(i.cursor().selectedText(), u"Hello"_s);

    i.cursor().removeSelectedText();
    QCOMPARE(i.text(), u" World"_s);
    QCOMPARE(i.cursor().position(), 0);
    QVERIFY(!i.cursor().hasSelection());
}

void TextItemTest::shouldTestTextManipulation()
{
    TextItem i;
    i.cursor().insertText(u"Hello"_s);
    QCOMPARE(i.text(), u"Hello"_s);

    i.cursor().setPosition(5);
    i.cursor().insertText(u" World"_s);
    QCOMPARE(i.text(), u"Hello World"_s);

    i.cursor().setPosition(5);
    i.cursor().setPosition(11, QTextCursor::KeepAnchor);
    i.cursor().removeSelectedText();
    QCOMPARE(i.text(), u"Hello"_s);
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
