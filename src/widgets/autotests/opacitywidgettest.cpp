/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "opacitywidgettest.hpp"
#include "properties/widgets/opacitywidget.hpp"
#include <QLabel>
#include <QSignalSpy>
#include <QSlider>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(OpacityWidgetTest)

OpacityWidgetTest::OpacityWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void OpacityWidgetTest::shouldHaveDefaultValues()
{
    const OpacityWidget w;
    QVERIFY(!w.name().isEmpty());
    QVERIFY(w.widget());
    QCOMPARE(w.widget()->objectName(), u"m_widget"_s);

    auto slider = w.widget()->findChild<QSlider *>(u"slider"_s);
    QVERIFY(slider);
    QCOMPARE(slider->objectName(), u"slider"_s);
    QCOMPARE(slider->minimum(), 0);
    QCOMPARE(slider->maximum(), 100);
    QCOMPARE(slider->singleStep(), 10);
    QCOMPARE(slider->pageStep(), 10);
    QCOMPARE(slider->tickInterval(), 10);
    QCOMPARE(slider->value(), 100);

    auto label = w.widget()->findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QCOMPARE(label->text(), u"100"_s);
}

void OpacityWidgetTest::shouldSnapOpacityValuesToDiscreteSteps()
{
    OpacityWidget w;
    const QSignalSpy spy(&w, &PropertyWidget::changed);

    auto slider = w.widget()->findChild<QSlider *>(u"slider"_s);
    QVERIFY(slider);

    slider->setValue(56);
    QCOMPARE(slider->value(), 60);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(w.value().value<int>(), 153);

    auto label = w.widget()->findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QCOMPARE(label->text(), u"60"_s);
}

void OpacityWidgetTest::shouldNotEmitChangedWhenSettingValueProgrammatically()
{
    OpacityWidget w;
    const QSignalSpy spy(&w, &PropertyWidget::changed);

    w.setValue(128);
    QCOMPARE(spy.count(), 0);

    auto slider = w.widget()->findChild<QSlider *>(u"slider"_s);
    QVERIFY(slider);
    QCOMPARE(slider->value(), 50);

    auto label = w.widget()->findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QCOMPARE(label->text(), u"50"_s);

    QCOMPARE(w.value().value<int>(), 128);
}

#include "moc_opacitywidgettest.cpp"
