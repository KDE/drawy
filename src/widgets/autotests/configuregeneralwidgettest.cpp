/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "configuregeneralwidgettest.hpp"
#include "dialog/configuregeneralwidget.hpp"
#include <QSpinBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfigureGeneralWidgetTest)
using namespace Qt::Literals::StringLiterals;
ConfigureGeneralWidgetTest::ConfigureGeneralWidgetTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ConfigureGeneralWidgetTest::shouldHaveDefaultValues()
{
    const ConfigureGeneralWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mAutoSave = w.findChild<QSpinBox *>(u"mAutoSave"_s);
    QVERIFY(mAutoSave);
    QCOMPARE(mAutoSave->value(), 0);
}

#include "moc_configuregeneralwidgettest.cpp"
