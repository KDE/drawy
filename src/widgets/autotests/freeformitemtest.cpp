/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "freeformitemtest.hpp"
#include "drawy_autotest_helper.hpp"
#include "item/freeform.hpp"

#include <QJsonObject>
#include <QTest>
QTEST_MAIN(FreeformItemTest)
using namespace Qt::Literals::StringLiterals;
FreeformItemTest::FreeformItemTest(QObject *parent)
    : QObject{parent}
{
}

void FreeformItemTest::shouldHaveDefaultValues()
{
    const FreeformItem i;
    QCOMPARE(i.type(), Item::Type::Freeform);
    const auto properties = QList<Property::Type>() << Property::Type::StrokeWidth << Property::Type::StrokeColor << Property::Type::Opacity
                                                    << Property::Type::StrokeStyle;
    QCOMPARE(i.propertyTypes().count(), 4);
    for (const auto &prop : properties) {
        QVERIFY(i.propertyTypes().contains(prop));
    }
}

void FreeformItemTest::shouldSerializeDefaultValue()
{
    const FreeformItem f;
    const QJsonObject obj = f.serialize();
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/freeform/"_s, ba, u"defaultvalue"_s);
}
#include "moc_freeformitemtest.cpp"
