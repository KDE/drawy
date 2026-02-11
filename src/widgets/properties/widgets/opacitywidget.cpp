/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "opacitywidget.hpp"
#include <QHBoxLayout>
#include <QLabel>
#include <KLocalizedString>
#include <QSlider>
using namespace Qt::Literals::StringLiterals;
OpacityWidget::OpacityWidget(QWidget *parent)
    : PropertyWidget(parent)
{
    m_widget = new QWidget{parent};
    m_widget->setObjectName(u"m_widget"_s);
    auto layout{new QHBoxLayout{m_widget}};
    layout->setContentsMargins({});

    mSlider = new QSlider(parent);
    mSlider->setObjectName(u"slider"_s);
    mSlider->setMinimum(1);
    mSlider->setMaximum(255);
    mSlider->setValue(255);
    mSlider->setOrientation(Qt::Horizontal);

    auto label = new QLabel(m_widget);
    label->setObjectName(u"label"_s);
    label->setText(QString::number(100));

    layout->addWidget(mSlider);
    layout->addWidget(label);
    m_widget->hide();

    connect(mSlider, &QSlider::valueChanged, this, [this, label](int v) {
        label->setText(QString::number(static_cast<int>((v / 255.0) * 100)));
        Q_EMIT changed(value());
    });
}

QString OpacityWidget::name() const
{
    return i18n("Opacity");
}

const Property OpacityWidget::value() const
{
    return Property{mSlider->value(), Property::Type::Opacity};
}

#include "moc_opacitywidget.cpp"
