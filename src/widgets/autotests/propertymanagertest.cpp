/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "propertymanagertest.hpp"
#include "properties/widgets/propertymanager.hpp"
#include <QTest>
QTEST_MAIN(PropertyManagerTest)

PropertyManagerTest::PropertyManagerTest(QObject *parent)
    : QObject{parent}
{
}

void PropertyManagerTest::shouldHaveDefaultValues()
{
    PropertyManager w;
    QVERIFY(w.widget(Property::Type::StrokeWidth));
    QVERIFY(w.widget(Property::Type::StrokeColor));
    QVERIFY(w.widget(Property::Type::StrokeStyle));
    QVERIFY(w.widget(Property::Type::EraserSize));
    QVERIFY(w.widget(Property::Type::FontSize));
    QVERIFY(w.widget(Property::Type::Actions));
    QVERIFY(w.widget(Property::Type::BackgroundColor));
    QVERIFY(w.widget(Property::Type::Opacity));
}

#include "moc_propertymanagertest.cpp"
