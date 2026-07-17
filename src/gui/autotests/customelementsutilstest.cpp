/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "customelementsutilstest.hpp"
#include "customelements/customelementsutils.hpp"
#include <QStandardPaths>
#include <QTest>
QTEST_GUILESS_MAIN(CustomElementsUtilsTest)
using namespace Qt::Literals::StringLiterals;
CustomElementsUtilsTest::CustomElementsUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void CustomElementsUtilsTest::shouldHaveDefaultValues()
{
    QCOMPARE(CustomElementsUtils::customElementsFilePath(), QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + u"/custom-elements/"_s);
}

#include "moc_customelementsutilstest.cpp"
