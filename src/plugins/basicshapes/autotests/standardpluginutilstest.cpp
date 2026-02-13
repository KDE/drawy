/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "standardpluginutilstest.hpp"
#include "basicshapespluginutils.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(StandardPluginUtilsTest)
using namespace Qt::Literals::StringLiterals;
StandardPluginUtilsTest::StandardPluginUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void StandardPluginUtilsTest::shouldHavePluginName()
{
    QCOMPARE(BasicShapesPluginUtils::pluginName(), u"standardforms"_s);
}

void StandardPluginUtilsTest::shouldConvertStandardFormPluginTypeToString()
{
    QCOMPARE(BasicShapesPluginUtils::convertStandardFormPluginTypeToString(BasicShapesPluginUtils::BasicShapesPluginType::Triangle), u"TRIANGLE"_s);
    QCOMPARE(BasicShapesPluginUtils::convertStandardFormPluginTypeToString(BasicShapesPluginUtils::BasicShapesPluginType::XBox), u"XBOX"_s);
}

void StandardPluginUtilsTest::shouldConvertStandardFormPluginTypeFromString()
{
    QCOMPARE(BasicShapesPluginUtils::convertStandardFormPluginTypeFromString(u"XBOX"_s), BasicShapesPluginUtils::BasicShapesPluginType::XBox);
    QCOMPARE(BasicShapesPluginUtils::convertStandardFormPluginTypeFromString(u"TRIANGLE"_s), BasicShapesPluginUtils::BasicShapesPluginType::Triangle);
    QCOMPARE(BasicShapesPluginUtils::convertStandardFormPluginTypeFromString(u"foo"_s), BasicShapesPluginUtils::BasicShapesPluginType::Unknown);
    QCOMPARE(BasicShapesPluginUtils::convertStandardFormPluginTypeFromString(QString()), BasicShapesPluginUtils::BasicShapesPluginType::Unknown);
}

#include "moc_standardpluginutilstest.cpp"
