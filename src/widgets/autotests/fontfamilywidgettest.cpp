/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "fontfamilywidgettest.hpp"
#include "properties/widgets/fontfamilywidget.hpp"
#include <QFontComboBox>
#include <QStandardPaths>
#include <QTest>
using namespace Qt::Literals::StringLiterals;

QTEST_MAIN(FontFamilyWidgetTest)
FontFamilyWidgetTest::FontFamilyWidgetTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void FontFamilyWidgetTest::shouldHaveDefaultValues()
{
    const auto parent = new QWidget();
    const FontFamilyWidget w(parent);
    QVERIFY(!w.name().isEmpty());

    auto m_fontComboBox = parent->findChild<QFontComboBox *>(u"m_fontComboBox"_s);
    QVERIFY(m_fontComboBox);
    QCOMPARE(m_fontComboBox->sizePolicy().horizontalPolicy(), QSizePolicy::Ignored);
    QCOMPARE(m_fontComboBox->sizePolicy().verticalPolicy(), QSizePolicy::Fixed);
    QCOMPARE(m_fontComboBox->currentFont().family(), u"Fuzzy Bubbles"_s);
}

#include "moc_fontfamilywidgettest.cpp"
