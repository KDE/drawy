/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "polygonitemtest.hpp"
#include "item/polygon.hpp"
#include <QTest>
QTEST_MAIN(PolygonItemTest)

PolygonItemTest::PolygonItemTest(QObject *parent)
    : QObject{parent}
{
}

void PolygonItemTest::shouldHaveDefaultValues()
{
}

#include "moc_polygonitemtest.cpp"
