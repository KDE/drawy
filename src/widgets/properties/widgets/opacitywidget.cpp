/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "opacitywidget.hpp"
#include <QSlider>
using namespace Qt::Literals::StringLiterals;
OpacityWidget::OpacityWidget(QWidget *parent)
    : PropertyWidget(parent)
{
    auto slider{new QSlider(parent)};
    slider->setObjectName(u"slider"_s);
    slider->setMinimum(1);
    slider->setMaximum(100);
    slider->setValue(100);

    slider->hide();
    m_widget = slider;

    connect(slider, &QSlider::valueChanged, this, [this]() {
        Q_EMIT changed(value());
    });
}

QString OpacityWidget::name() const
{
    return tr("Opacity");
}

const Property OpacityWidget::value() const
{
    return Property{static_cast<QSlider *>(m_widget)->value(), Property::Type::Opacity};
}

#include "moc_opacitywidget.cpp"
