/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "customelementsmanagertest.hpp"
#include "customelements/customelementsmanager.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(CustomElementsManagerTest)

CustomElementsManagerTest::CustomElementsManagerTest(QObject *parent)
    : QObject{parent}
{
}

void CustomElementsManagerTest::shouldHaveDefaultValues()
{
    const auto w = CustomElementsManager::self();
    QVERIFY(w->isEmpty());
}

#include "moc_customelementsmanagertest.cpp"
