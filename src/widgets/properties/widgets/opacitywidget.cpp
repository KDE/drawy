/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "opacitywidget.hpp"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QSignalBlocker>
#include <QSlider>
#include <algorithm>
using namespace Qt::Literals::StringLiterals;

namespace
{
constexpr int minimumAlpha = 1;
constexpr int maximumAlpha = 255;
constexpr int minimumPercent = 0;
constexpr int maximumPercent = 100;
constexpr int percentStep = 10;

int alphaToPercent(int alpha)
{
    const int boundedAlpha = std::clamp(alpha, minimumAlpha, maximumAlpha);
    return qRound((boundedAlpha * 100.0) / maximumAlpha);
}

int percentToAlpha(int percent)
{
    const int boundedPercent = std::clamp(percent, minimumPercent, maximumPercent);
    if (boundedPercent == minimumPercent) {
        return minimumAlpha;
    }
    if (boundedPercent == maximumPercent) {
        return maximumAlpha;
    }
    return std::clamp(qRound((boundedPercent / 100.0) * maximumAlpha), minimumAlpha, maximumAlpha);
}

int snapPercent(int percent)
{
    const int boundedPercent = std::clamp(percent, minimumPercent, maximumPercent);
    return ((boundedPercent + (percentStep / 2)) / percentStep) * percentStep;
}
} // namespace

OpacityWidget::OpacityWidget(QWidget *parent)
    : PropertyWidget(parent)
    , mSlider(new QSlider(parent))
{
    m_widget = new QWidget{parent};
    m_widget->setObjectName(u"m_widget"_s);
    auto layout{new QHBoxLayout{m_widget}};
    layout->setContentsMargins({});

    mSlider->setObjectName(u"slider"_s);
    mSlider->setMinimum(minimumPercent);
    mSlider->setMaximum(maximumPercent);
    mSlider->setSingleStep(percentStep);
    mSlider->setPageStep(percentStep);
    mSlider->setTickInterval(percentStep);
    mSlider->setValue(maximumPercent);
    mSlider->setOrientation(Qt::Horizontal);

    auto label = new QLabel(m_widget);
    label->setObjectName(u"label"_s);
    label->setText(QString::number(maximumPercent));

    layout->addWidget(mSlider);
    layout->addWidget(label);
    m_widget->hide();

    connect(mSlider, &QSlider::valueChanged, this, [this, label](int v) {
        const int snappedPercent = snapPercent(v);
        if (snappedPercent != v) {
            const QSignalBlocker blocker(mSlider);
            mSlider->setValue(snappedPercent);
        }

        label->setText(QString::number(snappedPercent));
        Q_EMIT changed(value());
    });
}

void OpacityWidget::setValue(const QVariant &value)
{
    const int snappedPercent = snapPercent(alphaToPercent(value.toInt()));
    const QSignalBlocker blocker(mSlider);
    mSlider->setValue(snappedPercent);
    auto label = m_widget->findChild<QLabel *>(u"label"_s);
    if (label) {
        label->setText(QString::number(snappedPercent));
    }
}

QString OpacityWidget::name() const
{
    return i18n("Opacity");
}

Property OpacityWidget::value() const
{
    return Property{percentToAlpha(mSlider->value()), Property::Type::Opacity};
}

#include "moc_opacitywidget.cpp"
