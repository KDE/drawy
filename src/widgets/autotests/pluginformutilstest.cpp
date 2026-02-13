/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "pluginformutilstest.hpp"
#include "pluginform/pluginformutils.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(PluginFormUtilsTest)
using namespace Qt::Literals::StringLiterals;
PluginFormUtilsTest::PluginFormUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void PluginFormUtilsTest::shouldDefaultValues()
{
    QCOMPARE(PluginFormUtils::pluginNameStr(), u"pluginName"_s);
    QCOMPARE(PluginFormUtils::customObjectNameStr(), u"customObjectName"_s);
}

#include "moc_pluginformutilstest.cpp"
