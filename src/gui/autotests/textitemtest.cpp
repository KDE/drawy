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
    const auto properties = QList<Property::Type>() << Property::Type::FontSize << Property::Type::FontStyle << Property::Type::StrokeColor
                                                    << Property::Type::Opacity << Property::Type::FontFamily << Property::Type::TextAlignment;
    QCOMPARE(i.allPropertyTypes().count(), 6);
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
    obj.remove(u"html"_s);

    TextItem f2;
    f2.deserialize(obj);

    QCOMPARE(f2.text(), plainText);
}

void TextItemTest::shouldTestOldLoadNew()
{
    TextItem f;
    f.setId("acff679ae3c14260b56ef00f1d354883"_ba);
    f.createTextBox(QPointF(10, 10));
    const auto text = u"Hello World"_s;
    f.cursor().insertText(text);

    QJsonObject obj = f.serialize(-1);

    const QString versionOneResult = obj[u"text"_s].toString();

    TextItem f2;
    f2.deserialize(obj);
    const QString versionTwoResult = f2.text();

    QCOMPARE(versionOneResult, text);
    QCOMPARE(versionTwoResult, versionOneResult);
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

void TextItemTest::shouldTestUpdatePreedit()
{
    TextItem item;
    item.createTextBox(QPointF(0, 0));
    item.cursor().insertText(u"Hello"_s);
    item.setMode(TextItem::Mode::Edit);

    const QRectF initialBounds = item.boundingBox();
    const QString initialText = item.text();

    item.updatePreedit(u"preedit"_s, {});
    QCOMPARE(item.text(), initialText);
    QVERIFY(item.boundingBox().size().width() > initialBounds.size().width());

    item.updatePreedit({}, {});
    QCOMPARE(item.text(), initialText);
    QCOMPARE(item.boundingBox(), initialBounds);
}

void TextItemTest::shouldTestPerRangeFormatting()
{
    TextItem i;
    i.setMode(TextItem::Mode::Edit);
    i.cursor().insertText(u"Hello World"_s);

    i.cursor().setPosition(0);
    i.setProperty(Property::Type::FontStyle, Property(u"Bold"_s, Property::Type::FontStyle));
    QCOMPARE(i.property(Property::Type::FontStyle).value<QString>(), u"Bold"_s);
    QCOMPARE(i.cursor().charFormat().fontWeight(), QFont::Bold);

    i.cursor().setPosition(5, QTextCursor::KeepAnchor); // select "Hello"
    QCOMPARE(i.property(Property::Type::FontStyle).value<QString>(), u"Normal"_s);

    i.setProperty(Property::Type::FontStyle, Property(u"Italic"_s, Property::Type::FontStyle));
    QCOMPARE(i.property(Property::Type::FontStyle).value<QString>(), u"Italic"_s);

    i.cursor().setPosition(5);
    i.cursor().setPosition(11, QTextCursor::KeepAnchor); // select "World"
    QCOMPARE(i.property(Property::Type::FontStyle).value<QString>(), u"Normal"_s);
}

void TextItemTest::shouldTestAlignmentExpansion()
{
    TextItem i;
    i.setMode(TextItem::Mode::Edit);
    i.createTextBox(QPointF(0, 0));

    QRectF before = i.boundingBox();
    i.cursor().insertText(u"Hello"_s);
    QRectF after = i.boundingBox();
    QCOMPARE(before.left(), after.left());
    QVERIFY(after.right() > before.right());

    i.setProperty(Property::Type::TextAlignment, Property(u"AlignRight"_s, Property::Type::TextAlignment));
    before = i.boundingBox();
    i.cursor().insertText(u"Hello"_s);
    after = i.boundingBox();
    QCOMPARE(before.right(), after.right());
    QVERIFY(after.left() < before.left());

    i.setProperty(Property::Type::TextAlignment, Property(u"AlignCenter"_s, Property::Type::TextAlignment));
    before = i.boundingBox();
    i.cursor().insertText(u"Hello"_s);
    after = i.boundingBox();
    QCOMPARE(before.center().x(), after.center().x());
    QVERIFY(after.left() < before.left());
    QVERIFY(after.right() > before.right());
}

void TextItemTest::shouldTestAlignmentDeserialization()
{
    TextItem i;
    i.setMode(TextItem::Mode::Edit);
    i.createTextBox(QPointF(0, 0));
    i.setProperty(Property::Type::TextAlignment, Property(u"AlignRight"_s, Property::Type::TextAlignment));
    i.cursor().insertText(u"Hello World"_s);

    const QJsonObject obj = i.serialize(0);

    TextItem i2;
    i2.deserialize(obj);

    QCOMPARE(i2.boundingBox(), i.boundingBox());
    QCOMPARE(i2.property(Property::Type::TextAlignment).value<QString>(), u"AlignRight"_s);
}

#include "moc_textitemtest.cpp"
