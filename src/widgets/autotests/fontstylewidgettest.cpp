/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "fontstylewidgettest.hpp"
#include "properties/widgets/fontstylewidget.hpp"
#include <QHBoxLayout>
#include <QTest>
#include <QToolButton>

QTEST_MAIN(FontStyleWidgetTest)
using namespace Qt::Literals::StringLiterals;

FontStyleWidgetTest::FontStyleWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void FontStyleWidgetTest::shouldHaveDefaultValues()
{
    auto parent = new QWidget();
    const FontStyleWidget w(parent);
    QVERIFY(!w.name().isEmpty());

    auto m_widget = parent->findChild<QWidget *>(u"m_widget"_s);
    QVERIFY(m_widget);

    auto layout = m_widget->findChild<QHBoxLayout *>(u"layout"_s);
    QVERIFY(layout);

    auto m_bold = m_widget->findChild<QToolButton *>(u"m_bold"_s);
    QVERIFY(m_bold);
    QVERIFY(m_bold->isCheckable());
    QVERIFY(!m_bold->toolTip().isEmpty());

    auto m_italic = m_widget->findChild<QToolButton *>(u"m_italic"_s);
    QVERIFY(m_italic);
    QVERIFY(m_italic->isCheckable());
    QVERIFY(!m_italic->toolTip().isEmpty());

    auto m_underlined = m_widget->findChild<QToolButton *>(u"m_underlined"_s);
    QVERIFY(m_underlined);
    QVERIFY(m_underlined->isCheckable());
    QVERIFY(!m_underlined->toolTip().isEmpty());

    auto m_strikethrough = m_widget->findChild<QToolButton *>(u"m_strikethrough"_s);
    QVERIFY(m_strikethrough);
    QVERIFY(m_strikethrough->isCheckable());
    QVERIFY(!m_strikethrough->toolTip().isEmpty());
}

#include "moc_fontstylewidgettest.cpp"
