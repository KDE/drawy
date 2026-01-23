/*
 * SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "iconmanagertest.hpp"
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
    QCOMPARE(manager.iconPath(IconManager::Icon::TOOL_FREEFORM), u"draw-freehand"_s);

    // Missing icons for the moment
#if 0
    QCOMPARE(manager.iconPath(IconManager::Icon::PROPERTY_LINE_DASHED), u":/icons/fa-dark/property-line_dashed.svg"_s);
    QCOMPARE(manager.iconPath(IconManager::Icon::PROPERTY_LINE_DOTTED), u":/icons/fa-dark/property-line_dotted.svg"_s);
    QCOMPARE(manager.iconPath(IconManager::Icon::PROPERTY_LINE_SOLID), u":/icons/fa-dark/property-line_solid.svg"_s);
#endif
}

#include "moc_iconmanagertest.cpp"
