/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "arrowutilstest.hpp"
#include "item/arrow/arrowutils.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(ArrowUtilsTest)
using namespace Qt::Literals::StringLiterals;
ArrowUtilsTest::ArrowUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void ArrowUtilsTest::shouldConvertArrowTypeStringToArrowEnum()
{
}

void ArrowUtilsTest::shouldConvertArrowTypeEnumToString()
{
}

#include "moc_arrowutilstest.cpp"
