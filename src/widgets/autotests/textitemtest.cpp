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
    QCOMPARE(i.type(), Item::Type::Text);
    const auto properties = QList<Property::Type>() << Property::Type::FontSize << Property::Type::StrokeColor << Property::Type::Opacity;
    QCOMPARE(i.propertyTypes().count(), 3);
    for (const auto &prop : properties) {
        QVERIFY(i.propertyTypes().contains(prop));
    }
}

void TextItemTest::shouldSerializeDefaultValue()
{
    const TextItem f;
    const QJsonObject obj = f.serialize();
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
    const QJsonObject obj = f.serialize();
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/text/"_s, ba, name);
}
#endif
#include "moc_textitemtest.cpp"
