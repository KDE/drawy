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
    QCOMPARE(i.propertyTypes().count(), 3);
    for (const auto &prop : properties) {
        QVERIFY(i.propertyTypes().contains(prop));
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

// CI doesn't use same font as local => it failed
#if 0
void TextItemTest::shouldSerialize_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("fontSize");
    QTest::addColumn<QColor>("strokeColor");
    QTest::addRow("text1") << u"text1"_s << 10 << QColor(Qt::red);
    QTest::addRow("text2") << u"text2"_s << 15 << QColor(Qt::blue);
}

void TextItemTest::shouldSerialize()
{
    QFETCH(QString, name);
    QFETCH(int, fontSize);
    QFETCH(QColor, strokeColor);

    TextItem f;
    f.setProperty(Property::Type::StrokeColor, Property(strokeColor, Property::Type::StrokeColor));
    f.setProperty(Property::Type::FontSize, Property(fontSize, Property::Type::FontSize));
    const QJsonObject obj = f.serialize(-1); // Not define zorder yet
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/text/"_s, ba, name);
}
#endif

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

#include "moc_textitemtest.cpp"
