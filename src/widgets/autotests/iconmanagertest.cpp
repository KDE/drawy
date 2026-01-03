/*
 * SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "iconmanagertest.h"
#include "iconmanager/iconmanager.hpp"

#include <QTest>
QTEST_GUILESS_MAIN(IconManagerTest)
using namespace Qt::Literals::StringLiterals;
IconManagerTest::IconManagerTest(QObject *parent)
    : QObject{parent}
{
}

void IconManagerTest::shouldHaveDefaultValues()
{
    const IconManager manager;
    QCOMPARE(manager.iconPath(IconManager::TOOL_FREEFORM), u":/icons/fa-dark/tool-freeform.svg"_s);
}

#include "moc_iconmanagertest.cpp"
