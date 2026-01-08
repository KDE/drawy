/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "textitemtest.hpp"
#include "item/text.hpp"
#include <QTest>
QTEST_MAIN(TextItemTest)

TextItemTest::TextItemTest(QObject *parent)
    : QObject{parent}
{
}

void TextItemTest::shouldHaveDefaultValues()
{
    const TextItem i;
    QCOMPARE(i.type(), Item::Type::Text);
}

#include "moc_textitemtest.cpp"
