// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "autosavewidgettest.hpp"
#include "dialog/general/autosavewidget.hpp"
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QStandardPaths>
#include <QTest>

QTEST_MAIN(AutoSaveWidgetTest)
AutoSaveWidgetTest::AutoSaveWidgetTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

using namespace Qt::StringLiterals;
void AutoSaveWidgetTest::shouldHaveDefaultValues()
{
    QWidget *parent{};
    const AutoSaveWidget w{parent};

    auto m_delay{w.findChild<QSpinBox *>(u"m_delay"_s)};
    QCOMPARE(m_delay->value(), 0);
    QVERIFY(m_delay);

    auto m_path{w.findChild<QLineEdit *>(u"m_path"_s)};
    QCOMPARE(m_path->text(), u""_s);
    QVERIFY(m_path);

    auto m_enabled{w.findChild<QCheckBox *>(u"m_enabled"_s)};
    QCOMPARE(m_enabled->isChecked(), false);
    QVERIFY(m_enabled);
}

#include "moc_autosavewidgettest.cpp"
