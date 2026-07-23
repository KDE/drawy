/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "customelementslistviewtest.hpp"
#include "customelementswidget/customelementslistview.hpp"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(CustomElementsListViewTest)
CustomElementsListViewTest::CustomElementsListViewTest(QObject *parent)
    : QObject{parent}
{
}

void CustomElementsListViewTest::shouldHaveDefaultValues()
{
    const CustomElementsListView w(nullptr);
    // TODO add check
    Q_UNUSED(w)
}
